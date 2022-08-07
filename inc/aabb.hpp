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

#ifndef AABB_H
#define AABB_H

#include "ray.hpp"
#include "vec3.hpp"

namespace ronald {

/**
 * An Axis-Aligned Bounding Box. An AABB is simply a box inside which another
 * primitive or set of primitives can be completely contained. It is used for
 * the Bounding Volume Hierarchy collision acceleration data structure. We
 * define the AABB separately from the Primitive class because the AABB doesn't
 * need to return any information about the location and normal vector of the
 * intersection point. This way we can save a bit on virtual function calls and
 * unnecessary computation while traversing the BVH, which is the most
 * expensive operation of the entire program
 */
class AABB {
public:
  Vec3 min;
  Vec3 max;

  /**
   * Allow constructing a zeroed AABB
   */
  AABB() = default;

  /**
   * Construct an AABB from the given `min` and `max` vectors
   */
  AABB(const Vec3 &min_a, const Vec3 &max_a) : min(min_a), max(max_a){};

  /**
   * Check whether the given Ray intersects the AABB.
   */
  bool hit(const Ray &r, const Vec3 &inv_dir, float t_min, float t_max) const;

  /**
   * Return the axis along with the box is largest
   */
  size_t largest_extent() const;

  /**
   * Return a new AABB which completely contains the two provided bounding boxes
   */
  static AABB surrounding_box(const AABB &a, const AABB &b);

  /**
   * Stream insertion operator
   */
  friend std::ostream &operator<<(std::ostream &output, const AABB &a) {
    output << "{ " << a.min << ", " << a.max << " }";
    return output;
  }
};

} // namespace ronald

#endif // AABB_H
