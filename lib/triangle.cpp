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

#include "common.hpp"
#include "primitive.hpp"
#include "vec3.hpp"

namespace ronald {

Triangle::Triangle(const Vec3 &v0_a, const Vec3 &v1_a, const Vec3 &v2_a,
                   const float normal_a) {
  v0 = v0_a;
  v1 = v1_a;
  v2 = v2_a;
  edge1 = v1_a - v0_a;
  edge2 = v2_a - v0_a;
  normal = edge1.cross(edge2).normalize() * normal_a;
}

Triangle::Triangle(const object &obj) {
  const auto norm = get<float>(obj, "normal", "primitive");
  const auto vertices =
      get<std::array<std::array<float, 3>, 3>>(obj, "vertices", "primitive");

  v0 = Vec3(vertices[0]);
  v1 = Vec3(vertices[1]);
  v2 = Vec3(vertices[2]);

  edge1 = v1 - v0;
  edge2 = v2 - v0;
  normal = edge1.cross(edge2).normalize() * norm;
}

/**
 * Möller–Trumbore algorithm for triangle intersection
 * https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
 */
std::optional<Intersection> Triangle::hit(const Ray &r, const float t_min,
                                          const float t_max) const {
  const auto h = r.direction().cross(edge2);
  const auto a = edge1.dot(h);

  if (a > -EPSILON && a < EPSILON) {
    return std::nullopt;
  }

  const auto f = 1 / a;
  const auto s = r.origin() - v0;
  const auto u = f * s.dot(h);

  // this branch might be unpredictable, should check this
  if (u < 0.0 || u > 1.0) {
    return std::nullopt;
  }

  const auto q = s.cross(edge1);
  const auto v = f * r.direction().dot(q);

  // this might also be unpredictable, check it
  if (v < 0.0 || u + v > 1.0) {
    return std::nullopt;
  }

  const auto t = f * edge2.dot(q);

  if (t > EPSILON && t > t_min && t < t_max) {
    const auto point = r.origin() + r.direction() * t;
    return {{
        point,
        normal,
        t,
    }};
  }

  return std::nullopt;
}

AABB Triangle::aabb() const {
  const auto min_x =
      std::min(std::min(v0.x(), v1.x()), std::min(v1.x(), v2.x()));
  const auto min_y =
      std::min(std::min(v0.y(), v1.y()), std::min(v1.y(), v2.y()));
  const auto min_z =
      std::min(std::min(v0.z(), v1.z()), std::min(v1.z(), v2.z()));
  const auto max_x =
      std::max(std::max(v0.x(), v1.x()), std::max(v1.x(), v2.x()));
  const auto max_y =
      std::max(std::max(v0.y(), v1.y()), std::max(v1.y(), v2.y()));
  const auto max_z =
      std::max(std::max(v0.z(), v1.z()), std::max(v1.z(), v2.z()));

  // we will add `EPSILON` padding to the bbox to prevent zero-volume bboxes
  // when the triangle is axis-aligned
  constexpr float ep = 0.0001f;
  return AABB(Vec3(min_x - ep, min_y - ep, min_z - ep),
              Vec3(max_x + ep, max_y + ep, max_z + ep));
}

} // namespace ronald
