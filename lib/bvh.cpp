/*
 * Copyright © 2022 Jayden Chan. All rights reserved.
 *
 * Ronald is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * Ronald is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ronald. If not, see <https://www.gnu.org/licenses/>.
 */

#include "bvh.hpp"
#include "common.hpp"
#include "dbg.h"
#include "rand.hpp"
#include <algorithm>
#include <cstdlib>

namespace ronald {

BVH::BVH(const NodeType _type, const AABB &_bbox, BVHPair children)
    : type(_type), bbox(_bbox), data(std::move(children)) {}

BVH::BVH(const NodeType _type, const AABB &_bbox, const Object &obj)
    : type(_type), bbox(_bbox), data(obj){};

BVH BVH::build_bvh(std::vector<Object> &objs, size_t *total_nodes) {
  // choose the axis to split on. for now we will just split on
  // a random axis -- this is pretty suboptimal but it's very
  // easy so we'll leave it as is for now
  const auto axis = static_cast<size_t>(3.0 * random_float());

  if (objs.size() == 0) {
    throw std::runtime_error("invalid bvh length");
  }

  if (objs.size() == 1) {
    const auto obj = objs.front();
    const auto bb = obj.primitive->aabb();
    *total_nodes += 1;
    return BVH(NodeType::Leaf, bb, obj);
  }

  const auto m = objs.begin() + objs.size() / 2;
  // TODO: SAH split method
  // partition the elements in the vector according to the split criteria.
  // for now we will split at the midpoint of the minimums of the child bboxes.
  // this is not really optimal but it will get the job done for the time being
  std::nth_element(objs.begin(), m, objs.end(),
                   [axis](const auto &a, const auto &b) {
                     const auto box_left = a.primitive->aabb();
                     const auto box_right = b.primitive->aabb();

                     return box_left.min[axis] - box_right.min[axis] < 0.0;
                   });

  // split off the vector in two pieces at the partition point
  std::vector<Object> l_vec(objs.begin(), m);
  std::vector<Object> r_vec(m, objs.end());

  // construct the left and right subtrees
  auto left = std::make_unique<BVH>(build_bvh(l_vec, total_nodes));
  auto right = std::make_unique<BVH>(build_bvh(r_vec, total_nodes));

  *total_nodes += 1;
  const auto surrounding_box = AABB::surrounding_box(left->bbox, right->bbox);
  return BVH(NodeType::Internal, surrounding_box,
             std::make_pair(std::move(left), std::move(right)));
}

std::optional<Hit> BVH::intersect(const Ray &r, const float t_min,
                                  const float t_max) const {
  const auto inv_dir = 1.0f / r.direction().normalize();
  if (this->bbox.hit(r, inv_dir, t_min, t_max)) {
    // we are an internal node -- check both child nodes and continue
    // traversing down the tree
    if (this->type == NodeType::Internal) {
      const auto &[left, right] = std::get<BVHPair>(this->data);
      const auto left_hit = left->intersect(r, t_min, t_max);

      if (left_hit.has_value()) {
        const auto right_hit = right->intersect(r, t_min, left_hit->hit.t);
        return (!right_hit.has_value() || left_hit->hit.t < right_hit->hit.t)
                   ? left_hit
                   : right_hit;
      } else {
        return right->intersect(r, t_min, t_max);
      }
    }

    // we are a leaf node -- check if the ray intersects the leaf primitive
    const auto obj = std::get<Object>(this->data);
    const auto hit = obj.primitive->hit(r, t_min, t_max);
    if (hit.has_value()) {
      return {{
          .hit = hit.value(),
          .material = obj.material,
      }};
    }
  }

  // ray misses the current node entirely
  return std::nullopt;
}

FlatBVH::FlatBVH(std::vector<Object> &objs) {
  size_t total_nodes = 0;
  const auto bvh = BVH::build_bvh(objs, &total_nodes);
  const FlatBVHNode def = {
      .type = NodeType::Internal,
      .bbox = AABB(),
      .data = static_cast<size_t>(0),
      .secondChildOffset = 0,
  };

  nodes = std::vector<FlatBVHNode>(total_nodes, def);
  size_t offset = 0;
  recursive_flatten(bvh, &offset);
}

size_t FlatBVH::recursive_flatten(const BVH &node, size_t *offset) {
  FlatBVHNode *flatNode = &nodes[*offset];
  flatNode->bbox = node.aabb();
  flatNode->type = node.node_type();

  const size_t myOffset = *offset;
  *offset += 1;

  if (node.node_type() == NodeType::Leaf) {
    flatNode->data = std::get<Object>(node.get_data());
  } else {
    // Create interior flattened BVH node
    const auto &[left, right] = std::get<BVHPair>(node.get_data());
    recursive_flatten(*left, offset);
    flatNode->secondChildOffset = recursive_flatten(*right, offset);
  }

  return myOffset;
}

std::optional<Hit> FlatBVH::intersect(const Ray &r, const float t_min,
                                      const float t_max) const {
  std::optional<Hit> curr_hit = {{
      .hit = {Vec3::zeros(), Vec3::zeros(), 0.0},
      .material = nullptr,
  }};

  auto min_so_far = t_max;

  const auto inv_dir = 1.0f / r.direction().normalize();
  // Follow ray through BVH nodes to find primitive intersections
  size_t toVisitOffset = 0;
  size_t currentNodeIndex = 0;
  size_t nodesToVisit[64];
  while (true) {
    const FlatBVHNode *node = &nodes[currentNodeIndex];

    if (node->bbox.hit(r, inv_dir, t_min, t_max)) {
      if (node->type == NodeType::Leaf) {
        const auto obj = std::get<Object>(node->data);
        const auto hit_result = obj.primitive->hit(r, t_min, min_so_far);
        if (hit_result.has_value()) {
          curr_hit->hit = *hit_result;
          curr_hit->material = obj.material;
          min_so_far = hit_result->t;
        }

        if (toVisitOffset == 0) {
          break;
        }
        currentNodeIndex = nodesToVisit[--toVisitOffset];

      } else {
        nodesToVisit[toVisitOffset++] = node->secondChildOffset;
        currentNodeIndex += 1;
      }
    } else {
      if (toVisitOffset == 0) {
        break;
      }
      currentNodeIndex = nodesToVisit[--toVisitOffset];
    }
  }

  if (curr_hit->material != nullptr) {
    return curr_hit;
  }
  return std::nullopt;
}

} // namespace ronald
