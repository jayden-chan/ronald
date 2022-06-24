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
using ronald::Vec3;

// https://psgraphics.blogspot.com/2020/12/debugging-refraction-in-ray-tracer.html
TEST_CASE("Vector refraction E2E test", "") {
  const auto center = Vec3(0, 0, 0);
  const auto radius = 1.0f;
  const auto sphere = Sphere(center, radius);

  const auto t_min = 0.00001f;
  const auto t_max = 10000000000;

  auto ray_origin = Vec3(0, 0, 2);
  auto dir = Vec3(0, 0, -1);
  auto ray = Ray(ray_origin, dir);

  auto hit = sphere.hit(ray, t_min, t_max);
  auto refracted = vector_refract(ray.direction(), hit->normal, 1);

  REQUIRE(hit.has_value());
  REQUIRE(hit->normal == Vec3(0, 0, 1));
  REQUIRE(hit->point == Vec3(0, 0, 1));
  REQUIRE(refracted.has_value());
  REQUIRE(*refracted == Vec3(0, 0, -1));

  ray_origin = hit->point;
  dir = *refracted;
  ray = Ray(ray_origin, dir);

  hit = sphere.hit(ray, t_min, t_max);
  refracted = vector_refract(ray.direction(), -hit->normal, 1);

  REQUIRE(hit.has_value());
  REQUIRE(hit->normal == Vec3(0, 0, -1));
  REQUIRE(hit->point == Vec3(0, 0, -1));
  REQUIRE(refracted.has_value());
  REQUIRE(*refracted == Vec3(0, 0, -1));
}

// https://vertexwahn.de/2020/12/19/refraction/
TEST_CASE("vertexwahn refraction case A", "[refract][vertexwahn]") {
  const auto normal = Vec3(0, 1, 0);
  const auto incident = Vec3(-1, 1, 0);
  const auto ref_idx = 1.0f / 1.6f;

  const auto refracted = vector_refract(incident, normal, ref_idx);
  REQUIRE(refracted.has_value());
  const auto normalized = refracted->normalize();
  REQUIRE(normalized.x() == Approx(-0.441975594f).margin(0.001));
  REQUIRE(normalized.y() == Approx(-0.897027075f).margin(0.01));
}

TEST_CASE("vertexwahn refraction case B", "[refract][vertexwahn]") {
  const auto normal = Vec3(0, 1, 0);
  const auto incident = Vec3(0, -1, 0);
  const auto ref_idx = 1.0f / 1.6f;

  const auto refracted = vector_refract(incident, normal, ref_idx);
  REQUIRE(refracted.has_value());
  REQUIRE(refracted->normalize() == Vec3(0, -1, 0));
}

TEST_CASE("vertexwahn refraction case C", "[refract][vertexwahn]") {
  const auto normal = Vec3(0, 1, 0);
  const auto incident = Vec3(-1, 1, 0);
  const auto ref_idx = 1.6f / 1.0f;

  const auto refracted = vector_refract(incident, normal, ref_idx);
  REQUIRE(!refracted.has_value());
}
