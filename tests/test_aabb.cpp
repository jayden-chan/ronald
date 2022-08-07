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
#include "dbg.h"
#include "primitive.hpp"
#include "vec3_tests.hpp"
#include <catch2/catch.hpp>

using ronald::AABB;
using ronald::Ray;
using ronald::Sphere;
using ronald::Triangle;
using ronald::Vec3;

TEST_CASE("AABB Sphere basic tests", "[aabb]") {
  const auto sphere = Sphere(Vec3(0, 0, 0), 5.0f);
  const auto bbox = sphere.aabb();

  REQUIRE(bbox.min == Vec3(-5, -5, -5));
  REQUIRE(bbox.max == Vec3(5, 5, 5));
}

TEST_CASE("AABB Triangle basic tests", "[aabb]") {
  const auto v0 = Vec3(-5, 0, 0);
  const auto v1 = Vec3(5, 0, 0);
  const auto v2 = Vec3(0, 5, 0);
  const auto triangle = Triangle(v0, v1, v2, 1.0);
  const auto bbox = triangle.aabb();

  REQUIRE(bbox.min == Vec3(-5, 0, 0));
  REQUIRE(bbox.max == Vec3(5, 5, 0));
}

TEST_CASE("AABB Sphere intersection test", "[aabb]") {
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
  auto inv_dir = 1.0 / ray.direction().normalize();
  REQUIRE(sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, inv_dir, t_min, t_max));

  ray_origin = Vec3(4.5, 4.5, -10);
  dir = (Vec3(4.5, 4.5, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  inv_dir = 1.0 / ray.direction().normalize();
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, inv_dir, t_min, t_max));

  ray_origin = Vec3(4.9999f, 4.9999f, -10);
  dir = (Vec3(4.9999f, 4.9999f, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  inv_dir = 1.0 / ray.direction().normalize();
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, inv_dir, t_min, t_max));

  ray_origin = Vec3(0, -4.999999F, -10);
  dir = (Vec3(0, -4.999999F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  inv_dir = 1.0 / ray.direction().normalize();
  REQUIRE(sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, inv_dir, t_min, t_max));

  ray_origin = Vec3(0, -5.000001F, -10);
  dir = (Vec3(0, -5.000001F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  inv_dir = 1.0 / ray.direction().normalize();
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(!aabb.hit(ray, inv_dir, t_min, t_max));

  ray_origin = Vec3(4, 4, -10);
  dir = (Vec3(4, 4, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  inv_dir = 1.0 / ray.direction().normalize();
  REQUIRE(!sphere.hit(ray, t_min, t_max));
  REQUIRE(aabb.hit(ray, inv_dir, t_min, t_max));
}

TEST_CASE("AABB surrounding_box test", "[aabb]") {
  const auto s1 = Sphere(Vec3(3, 3, 0), 5.0f);
  const auto s2 = Sphere(Vec3(-3, -3, 0), 2.0f);

  const auto surrounding_box = AABB::surrounding_box(s1.aabb(), s2.aabb());

  REQUIRE(surrounding_box.min == Vec3(-5, -5, -5));
  REQUIRE(surrounding_box.max == Vec3(8, 8, 5));
}
