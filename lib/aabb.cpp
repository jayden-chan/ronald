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

#include "aabb.hpp"
namespace ronald {

bool AABB::hit(const Ray &r, float t_min, float t_max) const {
  // check all three axes for if the ray misses
  for (size_t axis = 0; axis < 3; ++axis) {

    const auto t0 =
        std::min((min[axis] - r.origin()[axis]) / r.direction()[axis],
                 (max[axis] - r.origin()[axis]) / r.direction()[axis]);

    const auto t1 =
        std::max((min[axis] - r.origin()[axis]) / r.direction()[axis],
                 (max[axis] - r.origin()[axis]) / r.direction()[axis]);

    const auto true_t_min = std::max(t0, t_min);
    const auto true_t_max = std::min(t1, t_max);

    if (true_t_max <= true_t_min) {
      return false;
    }
  }

  return true;
}
} // namespace ronald
