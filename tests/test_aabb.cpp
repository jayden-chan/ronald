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
#include "primitive.hpp"
#include <catch2/catch.hpp>

using ronald::Ray;
using ronald::Sphere;
using ronald::Vec3;

TEST_CASE("AABB Sphere test", "[aabb]") {
  // Sphere at the origin on the XY plane
  const auto center = Vec3(0, 0, 0);
  const auto radius = 5.0F;
  const auto sphere = Sphere(center, radius);

  const auto t_min = 0;
  const auto t_max = 10000000000;

  const auto aabb = sphere.aabb();

  // send a ray through the origin and ensure it hits
  auto ray_origin = Vec3(0, 0, -10);
  auto dir = (Vec3(0, 0, 0) - ray_origin).normalize();
  auto ray = Ray(ray_origin, dir);
  REQUIRE(sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, t_min, t_max));

  ray_origin = Vec3(4.5, 4.5, -10);
  dir = (Vec3(4.5, 4.5, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, t_min, t_max));

  ray_origin = Vec3(4.9999f, 4.9999f, -10);
  dir = (Vec3(4.9999f, 4.9999f, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, t_min, t_max));

  ray_origin = Vec3(0, -4.999999F, -10);
  dir = (Vec3(0, -4.999999F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, t_min, t_max));

  ray_origin = Vec3(0, -5.000001F, -10);
  dir = (Vec3(0, -5.000001F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(!aabb.hit(ray, t_min, t_max));

  ray_origin = Vec3(4, 4, -10);
  dir = (Vec3(4, 4, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, t_min, t_max));
}
