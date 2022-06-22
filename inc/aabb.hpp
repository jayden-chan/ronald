/*
 * Copyright © 2022 Jayden Chan. All rights reserved.
 *
 * path_tracer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * path_tracer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with path_tracer. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef AABB_H
#define AABB_H

#include "ray.hpp"
#include "vec3.hpp"

namespace path_tracer {

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
  Vec3 min;
  Vec3 max;

public:
  /**
   * Construct an AABB from the given `min` and `max` vectors
   */
  AABB(const Vec3 &min_a, const Vec3 &max_a) : min(min_a), max(max_a){};

  /**
   * Check whether the given Ray intersects the AABB.
   */
  bool hit(const Ray &r, float t_min, float t_max) const;
};

} // namespace path_tracer

#endif // AABB_H
