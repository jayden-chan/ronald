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

#include "vec3.hpp"

using path_tracer::Vec3;

TEST_CASE("Vector default constructor", "[vector]") {
  const Vec3 v;
  REQUIRE(v.x() == 0);
  REQUIRE(v.y() == 0);
  REQUIRE(v.z() == 0);
}

TEST_CASE("Vector initialization", "[vector]") {
  const auto v = Vec3(1, 2, 3);
  REQUIRE(v.x() == 1);
  REQUIRE(v.y() == 2);
  REQUIRE(v.z() == 3);
  REQUIRE(v == Vec3(1, 2, 3));
}

TEST_CASE("Vector zeros", "[vector]") {
  const auto v = Vec3::zeros();
  REQUIRE(v.x() == 0);
  REQUIRE(v.y() == 0);
  REQUIRE(v.z() == 0);
}

TEST_CASE("Vector ones", "[vector]") {
  const auto v = Vec3::ones();
  REQUIRE(v.x() == 1);
  REQUIRE(v.y() == 1);
  REQUIRE(v.z() == 1);
}

TEST_CASE("Vector normalize", "[vector]") {
  const auto v = Vec3(1, 2, 3);
  const auto result = v.normalize();
  REQUIRE(result.x() == Approx(0.2672612));
  REQUIRE(result.y() == Approx(0.5345225));
  REQUIRE(result.z() == Approx(0.8017837));
  REQUIRE(v == Vec3(1, 2, 3));
}

TEST_CASE("Vector inv mag", "[vector]") {
  const auto mag = Vec3(1, 2, 3).inv_mag();
  REQUIRE(mag == Approx(1.0f / 3.74165738677394138558));
}

TEST_CASE("Vector length", "[vector]") {
  const auto mag = Vec3(1, 2, 3).length();
  REQUIRE(mag == Approx(3.74165738677394138558));
}

TEST_CASE("Vector length squared", "[vector]") {
  const auto mag = Vec3(1, 2, 3).length_squared();
  REQUIRE(mag == Approx(14));
}

TEST_CASE("Vector dot product", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  const auto dot = v1.dot(v2);
  REQUIRE(dot == Approx(32));
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("Vector cross product", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  const auto cross = v1.cross(v2);
  REQUIRE(cross == Vec3(-3, 6, -3));
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("Vector += Vector", "[vector]") {
  auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  v1 += v2;
  REQUIRE(v1 == Vec3(5, 7, 9));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("Vector *= float", "[vector]") {
  auto v1 = Vec3(1, 2, 3);
  v1 *= 10;
  REQUIRE(v1 == Vec3(10, 20, 30));
}

TEST_CASE("Vector *= Vector", "[vector]") {
  auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  v1 *= v2;
  REQUIRE(v1 == Vec3(4, 10, 18));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("Vector /= float", "[vector]") {
  auto v1 = Vec3(10, 20, 30);
  v1 /= 2;
  REQUIRE(v1 == Vec3(5, 10, 15));
}

TEST_CASE("-Vector", "[vector]") {
  auto v1 = Vec3(1, 2, 3);
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(-v1 == Vec3(-1, -2, -3));
}

TEST_CASE("Vector operator[]", "[vector]") {
  auto v1 = Vec3(1, 2, 3);
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(v1[0] == 1);
  REQUIRE(v1[1] == 2);
  REQUIRE(v1[2] == 3);
}

TEST_CASE("Vector + Vector", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  const auto result = v1 + v2;
  REQUIRE(result == Vec3(5, 7, 9));
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("Vector + float", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto c = 1.5;
  const auto result = v1 + c;
  REQUIRE(result == Vec3(2.5, 3.5, 4.5));
  REQUIRE(v1 == Vec3(1, 2, 3));
}

TEST_CASE("Vector - Vector", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  const auto result = v2 - v1;
  REQUIRE(result == Vec3(3, 3, 3));
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("Vector * Vector", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto v2 = Vec3(4, 5, 6);
  const auto result = v1 * v2;
  REQUIRE(result == Vec3(4, 10, 18));
  REQUIRE(v1 == Vec3(1, 2, 3));
  REQUIRE(v2 == Vec3(4, 5, 6));
}

TEST_CASE("float * Vector", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto c = 2.0f;
  const auto result = c * v1;
  REQUIRE(result == Vec3(2, 4, 6));
  REQUIRE(v1 == Vec3(1, 2, 3));
}

TEST_CASE("Vector * float", "[vector]") {
  const auto v1 = Vec3(1, 2, 3);
  const auto c = 2.0f;
  const auto result = v1 * c;
  REQUIRE(result == Vec3(2, 4, 6));
  REQUIRE(v1 == Vec3(1, 2, 3));
}

TEST_CASE("Vector / float", "[vector]") {
  const auto v1 = Vec3(10, 20, 30);
  const auto c = 2.0f;
  const auto result = v1 / c;
  REQUIRE(result == Vec3(5, 10, 15));
  REQUIRE(v1 == Vec3(10, 20, 30));
}

TEST_CASE("Vector / Vector", "[vector]") {
  const auto v1 = Vec3(10, 20, 30);
  const auto v2 = Vec3(1, 2, 3);
  const auto result = v1 / v2;
  REQUIRE(result == Vec3(10, 10, 10));
  REQUIRE(v1 == Vec3(10, 20, 30));
  REQUIRE(v2 == Vec3(1, 2, 3));
}
