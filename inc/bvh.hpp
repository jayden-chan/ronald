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

#include "scene.hpp"
#include <vector>

namespace ronald {

class BVH;
using BVHPair = std::pair<std::shared_ptr<BVH>, std::shared_ptr<BVH>>;
using BVHData = std::variant<BVHPair, Object>;

enum class NodeType { Internal, Leaf };

class BVH {
private:
  NodeType type;
  AABB bbox;
  BVHData data = {};

public:
  [[nodiscard]] static BVH build_bvh(std::vector<Object> &objs);
  BVH(const NodeType type, const AABB bbox, const BVHPair children);
  BVH(const NodeType type, const AABB bbox, const Object &obj);

  [[nodiscard]] std::optional<Intersection> intersect(const Ray &r, float t_min,
                                                      float t_max) const;
};

} // namespace ronald

#endif // BVH_H
