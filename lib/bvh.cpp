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

namespace ronald {

BVH::BVH(const NodeType _type, const AABB &_bbox, BVHPair children)
    : type(_type), bbox(_bbox), data(std::move(children)) {}

BVH::BVH(const NodeType _type, const AABB &_bbox, const Object &obj)
    : type(_type), bbox(_bbox), data(obj){};

BVH BVH::build_bvh(std::vector<Object> &objs) {
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
    dbg(bb);
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
  dbg(l_vec.size());
  auto left = std::make_unique<BVH>(build_bvh(l_vec));
  dbg(r_vec.size());
  auto right = std::make_unique<BVH>(build_bvh(r_vec));

  const auto surrounding_box = AABB::surrounding_box(left->bbox, right->bbox);
  return BVH(NodeType::Internal, surrounding_box,
             std::make_pair(std::move(left), std::move(right)));
}

std::optional<Hit> BVH::intersect(const Ray &r, const float t_min,
                                  const float t_max) const {
  if (this->bbox.hit(r, t_min, t_max)) {
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

} // namespace ronald
