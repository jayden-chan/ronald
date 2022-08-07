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

#ifndef BVH_H
#define BVH_H

#include "common.hpp"
#include <vector>

namespace ronald {

class BVH;
using BVHPair = std::pair<std::unique_ptr<BVH>, std::unique_ptr<BVH>>;
using BVHData = std::variant<BVHPair, Object>;

enum class NodeType { Internal, Leaf };

class BVH {
private:
  NodeType type;
  AABB bbox;
  BVHData data = {};

  /**
   * Construct a BVH with the given node type, bbox, and child nodes
   */
  [[nodiscard]] BVH(const NodeType type, const AABB &bbox,
                    const BVHPair children);

  /**
   * Construct a BVH with the given node type, bbox, and object leaf node
   */
  [[nodiscard]] BVH(const NodeType type, const AABB &bbox, const Object &obj);

public:
  /**
   * Construct a BVH from the given vector of objects
   */
  [[nodiscard]] static BVH build_bvh(std::vector<Object> &objs,
                                     size_t *total_nodes);

  /**
   * Test if a ray intersects the BVH
   */
  [[nodiscard]] std::optional<Hit> intersect(const Ray &r, float t_min,
                                             float t_max) const;

  /**
   * Get the bbox for this node of the BVH
   */
  [[nodiscard]] AABB aabb() const { return this->bbox; }

  /**
   * Get the type of node
   */
  [[nodiscard]] NodeType node_type() const { return this->type; }

  /**
   * Get the node data
   */
  [[nodiscard]] BVHData const &get_data() const { return this->data; }
};

class FlatBVH {
  struct FlatBVHNode {
    NodeType type;
    AABB bbox;
    std::variant<size_t, Object> data;
    size_t secondChildOffset;
  };

  std::vector<FlatBVHNode> nodes;

  /**
   * Recursively flatten the given BVH into a FlatBVH
   */
  size_t recursive_flatten(const BVH &node, size_t *offset);

public:
  /**
   * Construct a new FlatBVH from the given scene objects
   */
  [[nodiscard]] explicit FlatBVH(std::vector<Object> &objs);

  /**
   * Test if a ray intersects the BVH
   */
  [[nodiscard]] std::optional<Hit> intersect(const Ray &r, float t_min,
                                             float t_max) const;
};

} // namespace ronald

#endif // BVH_H
