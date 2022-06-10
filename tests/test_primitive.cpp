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

#include <catch2/catch.hpp>

#include "primitive.hpp"
#include "ray.hpp"
#include "vec3.hpp"

TEST_CASE("Ray/Sphere intersection", "[primitive][ray][sphere]") {
  // Sphere at the origin on the XY plane
  auto const center = Vec3(0, 0, 0);
  auto const radius = 5.0F;
  auto const sphere = Sphere(center, radius);

  auto const t_min = 0;
  auto const t_max = 10000000000;

  // send a ray through the origin and ensure it hits
  auto ray_origin = Vec3(0, 0, -10);
  auto dir = (Vec3(0, 0, 0) - ray_origin).normalize();
  auto ray = Ray(ray_origin, dir);
  REQUIRE(sphere.hit(ray, t_min, t_max));

  ray_origin = Vec3(0, -5.000001F, -10);
  dir = (Vec3(0, -5.000001F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!sphere.hit(ray, t_min, t_max));

  ray_origin = Vec3(0, -4.999999F, -10);
  dir = (Vec3(0, -4.999999F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(sphere.hit(ray, t_min, t_max));

  ray_origin = Vec3(4, 4, -10);
  dir = (Vec3(4, 4, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!sphere.hit(ray, t_min, t_max));

  // generate a bunch of random rays through the origin and
  // ensure they all hit
  for (int i = 0; i < 10; i++) {
    auto const rand_point = Vec3::rand() * 1000;
    auto const dir_to_origin = (Vec3(0, 0, 0) - rand_point).normalize();
    auto const r = Ray(rand_point, dir_to_origin);
    INFO("Expecting vec to hit: " << r);
    auto const hit = sphere.hit(r, t_min, t_max);
    REQUIRE(hit);
  }

  // generate a bunch of random rays and ensure they all miss.
  // technically there is a chance that one of these could hit but
  // it's very unlikely. probably still bad testing practice but oh well
  for (int i = 0; i < 10; i++) {
    auto const r1 = Vec3::rand() * 10000;
    auto const r2 = Vec3::rand() * 10000;
    auto const r_dir = (r1 - r2).normalize();
    auto const r = Ray(r1, r_dir);
    INFO("Expecting vec to miss: " << r);
    auto const hit = sphere.hit(r, t_min, t_max);
    REQUIRE(!hit);
  }
}

TEST_CASE("Ray/Triangle intersection", "[primitive][ray][triangle]") {
  // Triangle at the origin on the XY plane
  auto const v0 = Vec3(0, 5, 0);
  auto const v1 = Vec3(-5, -5, 0);
  auto const v2 = Vec3(5, -5, 0);
  auto const triangle = Triangle(v0, v1, v2, Vec3(0, 0, -1));

  auto const t_min = 0;
  auto const t_max = 10000000000;

  // send a ray through the origin and ensure it hits
  auto ray_origin = Vec3(0, 0, -10);
  auto dir = (Vec3(0, 0, 0) - ray_origin).normalize();
  auto ray = Ray(ray_origin, dir);
  REQUIRE(triangle.hit(ray, t_min, t_max));

  // send some more manually-tuned rays and check their result
  ray_origin = Vec3(2, 2, -10);
  dir = (Vec3(2, 2, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!triangle.hit(ray, t_min, t_max));

  ray_origin = Vec3(-4.9999F, -4.9999F, -10);
  dir = (Vec3(-4.9999F, -4.9999F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(triangle.hit(ray, t_min, t_max));

  ray_origin = Vec3(10, 0, 10);
  dir = Vec3(-1, 0, 0);
  ray = Ray(ray_origin, dir);
  REQUIRE(!triangle.hit(ray, t_min, t_max));

  ray_origin = Vec3(-2.6F, 0, -10);
  dir = (Vec3(-2.6F, 0, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!triangle.hit(ray, t_min, t_max));

  ray_origin = Vec3(0, -5.000001F, -10);
  dir = (Vec3(0, -5.000001F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(!triangle.hit(ray, t_min, t_max));

  ray_origin = Vec3(0, -4.999999F, -10);
  dir = (Vec3(0, -4.999999F, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, dir);
  REQUIRE(triangle.hit(ray, t_min, t_max));

  // generate a bunch of random rays through the origin and
  // ensure they all hit
  for (int i = 0; i < 10; i++) {
    auto const rand_point = Vec3::rand() * 1000;
    auto const dir_to_origin = (Vec3(0, 0, 0) - rand_point).normalize();
    auto const r = Ray(rand_point, dir_to_origin);
    INFO("Expecting vec to hit: " << r);
    auto const hit = triangle.hit(r, t_min, t_max);
    REQUIRE(hit);
  }

  // generate a bunch of random rays and ensure they all miss.
  // technically there is a chance that one of these could hit but
  // it's very unlikely. probably still bad testing practice but oh well
  for (int i = 0; i < 10; i++) {
    auto const r1 = Vec3::rand() * 10000;
    auto const r2 = Vec3::rand() * 10000;
    auto const r_dir = (r1 - r2).normalize();
    auto const r = Ray(r1, r_dir);
    INFO("Expecting vec to miss: " << r);
    auto const hit = triangle.hit(r, t_min, t_max);
    REQUIRE(!hit);
  }
}
