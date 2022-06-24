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
#include "vec3_tests.hpp"

using ronald::Ray;
using ronald::Sphere;
using ronald::Triangle;
using ronald::Vec3;

TEST_CASE("Ray/Sphere intersection", "[primitive][ray][sphere]") {
  // Sphere at the origin on the XY plane
  const auto center = Vec3(0, 0, 0);
  const auto radius = 5.0F;
  const auto sphere = Sphere(center, radius);

  const auto t_min = 0;
  const auto t_max = 10000000000;

  // send a ray through the origin and ensure it hits
  auto ray_origin = Vec3(0, 0, -10);
  auto ray_dest = Vec3(0, 0, 0);
  auto ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  auto hitr = sphere.hit(ray, t_min, t_max);
  REQUIRE(hitr.has_value());

  ray_origin = Vec3(0, -5.000001F, -10);
  ray_dest = Vec3(0, -5.000001F, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = sphere.hit(ray, t_min, t_max);
  REQUIRE(!hitr.has_value());

  ray_origin = Vec3(0, -4.999999F, -10);
  ray_dest = Vec3(0, -4.999999F, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = sphere.hit(ray, t_min, t_max);
  REQUIRE(hitr.has_value());

  ray_origin = Vec3(4, 4, -10);
  ray_dest = Vec3(4, 4, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = sphere.hit(ray, t_min, t_max);
  REQUIRE(!hitr.has_value());

  // generate a bunch of random rays through the origin and
  // ensure they all hit
  for (int i = 0; i < 10; i++) {
    const auto rand_point = Vec3::rand() * 1000;
    const auto dir_to_origin = (Vec3(0, 0, 0) - rand_point).normalize();
    const auto r = Ray(rand_point, dir_to_origin);
    INFO("Expecting vec to hit: " << r);
    const auto hit = sphere.hit(r, t_min, t_max);
    REQUIRE(hit);
  }

  // generate a bunch of random rays and ensure they all miss.
  // technically there is a chance that one of these could hit but
  // it's very unlikely. probably still bad testing practice but oh well
  for (int i = 0; i < 10; i++) {
    const auto r1 = Vec3::rand() * 10000;
    const auto r2 = Vec3::rand() * 10000;
    const auto r_dir = (r1 - r2).normalize();
    const auto r = Ray(r1, r_dir);
    INFO("Expecting vec to miss: " << r);
    const auto hit = sphere.hit(r, t_min, t_max);
    REQUIRE(!hit);
  }
}

TEST_CASE("Ray/Sphere internal intersection", "[primitive][ray][sphere]") {
  // Sphere at the origin on the XY plane
  const auto center = Vec3(0, 0, 0);
  const auto radius = 5.0F;
  const auto sphere = Sphere(center, radius);

  const auto t_min = 0;
  const auto t_max = 10000000000;

  const auto ray_origin = Vec3(0, 0, 0);
  const auto dir = Vec3(0, 0, -1);
  const auto ray = Ray(ray_origin, dir);
  const auto hit = sphere.hit(ray, t_min, t_max);
  REQUIRE(hit.has_value());
  REQUIRE(hit->point == Vec3(0, 0, -5));
  REQUIRE(hit->normal == Vec3(0, 0, -1));
}

TEST_CASE("Ray/Triangle intersection", "[primitive][ray][triangle]") {
  // Triangle at the origin on the XY plane
  const auto v0 = Vec3(0, 5, 0);
  const auto v1 = Vec3(-5, -5, 0);
  const auto v2 = Vec3(5, -5, 0);
  const auto triangle = Triangle(v0, v1, v2, -1);

  const auto t_min = 0;
  const auto t_max = 10000000000;

  // send a ray through the origin and ensure it hits
  auto ray_origin = Vec3(0, 0, -10);
  auto ray_dest = Vec3(0, 0, 0);
  auto ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  auto hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(hitr.has_value());

  // send some more manually-tuned rays and check their result
  ray_origin = Vec3(2, 2, -10);
  ray_dest = Vec3(2, 2, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(!hitr.has_value());

  ray_origin = Vec3(-4.9999F, -4.9999F, -10);
  ray_dest = Vec3(-4.9999F, -4.9999F, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(hitr.has_value());

  ray_origin = Vec3(10, 0, 10);
  ray_dest = Vec3(0, 0, 10);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(!hitr.has_value());

  ray_origin = Vec3(-2.6F, 0, -10);
  ray_dest = Vec3(-2.6F, 0, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(!hitr.has_value());

  ray_origin = Vec3(0, -5.00001F, -10);
  ray_dest = Vec3(0, -5.00001F, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(!hitr.has_value());

  ray_origin = Vec3(0, -4.999999F, -10);
  ray_dest = Vec3(0, -4.999999F, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  hitr = triangle.hit(ray, t_min, t_max);
  REQUIRE(hitr.has_value());

  // generate a bunch of random rays through the origin and
  // ensure they all hit
  for (int i = 0; i < 10; i++) {
    const auto rand_point = Vec3::rand() * 1000;
    const auto dir_to_origin = (Vec3(0, 0, 0) - rand_point).normalize();
    const auto r = Ray(rand_point, dir_to_origin);
    INFO("Expecting vec to hit: " << r);
    const auto hit = triangle.hit(r, t_min, t_max);
    REQUIRE(hit);
  }

  // generate a bunch of random rays and ensure they all miss.
  // technically there is a chance that one of these could hit but
  // it's very unlikely. probably still bad testing practice but oh well
  for (int i = 0; i < 10; i++) {
    const auto r1 = Vec3::rand() * 10000;
    const auto r2 = Vec3::rand() * 10000;
    const auto r_dir = (r1 - r2).normalize();
    const auto r = Ray(r1, r_dir);
    INFO("Expecting vec to miss: " << r);
    const auto hit = triangle.hit(r, t_min, t_max);
    REQUIRE(!hit);
  }
}
