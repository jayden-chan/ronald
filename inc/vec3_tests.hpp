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

#include "vec3.hpp"
#include <catch2/catch.hpp>

namespace ronald {

/**
 * Vectors will be considered equal if all of their elements are
 * within `EPSILON` of each other. This function is used for unit testing
 * only for now
 */
[[nodiscard]] inline bool operator==(const ronald::Vec3 &lhs,
                                     const ronald::Vec3 &rhs) {
  const auto target = Approx(0).margin(0.002);
  return (lhs.x() - rhs.x() == target && lhs.y() - rhs.y() == target &&
          lhs.z() - rhs.z() == target);
}

} // namespace ronald
