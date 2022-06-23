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

#include <catch2/catch.hpp>

#include "primitive.hpp"
#include "ray.hpp"
#include "vec3.hpp"

using Ronald::Vec3;

TEST_CASE("Vector reflection test", "[vector][reflect]") {
  const auto v1 = Vec3(1, 1, 0);
  const auto normal = Vec3(0, 1, 0);
  const auto reflected = vector_reflect(v1, normal);

  REQUIRE(reflected == Vec3(1, -1, 0));
}
