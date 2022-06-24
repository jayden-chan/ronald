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

#include "bvh.hpp"
#include "common.hpp"
#include "material.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include "vec3_tests.hpp"

#include <catch2/catch.hpp>

using ronald::AABB;
using ronald::BVH;
using ronald::Dielectric;
using ronald::NodeType;
using ronald::Object;
using ronald::Ray;
using ronald::Sphere;
using ronald::Vec3;

constexpr auto T_MIN = 0;
constexpr auto T_MAX = 10000000000;

TEST_CASE("Basic BVH construction test", "[bvh]") {
  const auto s1 = Sphere(Vec3(3, 3, 0), 5.0f);
  const auto s2 = Sphere(Vec3(-3, -3, 0), 2.0f);
  const auto mat =
      std::make_shared<Dielectric>(Dielectric(1.52f, Vec3::ones()));

  std::vector<Object> objs = {
      {.primitive = std::make_shared<Sphere>(s1), .material = mat},
      {.primitive = std::make_shared<Sphere>(s2), .material = mat}};

  const auto bvh = BVH::build_bvh(objs);
  REQUIRE(bvh.node_type() == NodeType::Internal);

  const auto surrounding_box = AABB::surrounding_box(s1.aabb(), s2.aabb());
  REQUIRE(bvh.aabb().min == surrounding_box.min);
  REQUIRE(bvh.aabb().max == surrounding_box.max);
}

TEST_CASE("Basic BVH intersection test", "[bvh]") {
  const auto s1 = Sphere(Vec3(3, 3, 0), 5.0f);
  const auto s2 = Sphere(Vec3(-3, -3, 0), 2.0f);
  const auto mat =
      std::make_shared<Dielectric>(Dielectric(1.52f, Vec3::ones()));

  std::vector<Object> objs = {
      {.primitive = std::make_shared<Sphere>(s1), .material = mat},
      {.primitive = std::make_shared<Sphere>(s2), .material = mat}};

  const auto bvh = BVH::build_bvh(objs);
  REQUIRE(bvh.node_type() == NodeType::Internal);

  const auto surrounding_box = AABB::surrounding_box(s1.aabb(), s2.aabb());
  REQUIRE(bvh.aabb().min == surrounding_box.min);
  REQUIRE(bvh.aabb().max == surrounding_box.max);

  // test hit sphere 1
  auto ray_origin = Vec3(3, 3, -10);
  auto ray_dest = Vec3(3, 3, 0);
  auto ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  auto manual_hit_result = s1.hit(ray, T_MIN, T_MAX);
  auto expected_hit_point = Vec3(3, 3, -5);
  REQUIRE(manual_hit_result.has_value());
  REQUIRE(manual_hit_result->point == expected_hit_point);

  auto bvh_hit_result = bvh.intersect(ray, T_MIN, T_MAX);
  REQUIRE(bvh_hit_result.has_value());
  REQUIRE(bvh_hit_result->hit.point == expected_hit_point);

  // test hit sphere 2
  ray_origin = Vec3(-3, -3, 10);
  ray_dest = Vec3(-3, -3, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  manual_hit_result = s2.hit(ray, T_MIN, T_MAX);
  expected_hit_point = Vec3(-3, -3, 2);
  REQUIRE(manual_hit_result.has_value());
  REQUIRE(manual_hit_result->point == expected_hit_point);

  bvh_hit_result = bvh.intersect(ray, T_MIN, T_MAX);
  REQUIRE(bvh_hit_result.has_value());
  REQUIRE(bvh_hit_result->hit.point == expected_hit_point);

  // test miss both spheres
  ray_origin = Vec3(0, -2.5, 10);
  ray_dest = Vec3(0, -2.5, 0);
  ray = Ray(ray_origin, (ray_dest - ray_origin).normalize());
  manual_hit_result = s1.hit(ray, T_MIN, T_MAX);
  REQUIRE(!manual_hit_result.has_value());
  manual_hit_result = s2.hit(ray, T_MIN, T_MAX);
  REQUIRE(!manual_hit_result.has_value());

  bvh_hit_result = bvh.intersect(ray, T_MIN, T_MAX);
  REQUIRE(!bvh_hit_result.has_value());
}
