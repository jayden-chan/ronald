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

#include "math.hpp"

namespace path_tracer {

Vec3 random_in_unit_disk() {
  Vec3 v1, v2, p;

  do {
    v1 = Vec3(random_float(), random_float(), 0);
    v2 = Vec3(1, 1, 0);
    p = 2 * v1 - v2;
  } while (p.dot(p) >= 1);

  return p;
}

Vec3 random_in_unit_sphere() {
  Vec3 p;

  do {
    p = 2 * Vec3::rand() - Vec3::ones();
  } while (p.dot(p) >= 1);

  return p;
}

Vec3 random_on_unit_sphere() { return random_in_unit_sphere().normalize(); }

} // namespace path_tracer
