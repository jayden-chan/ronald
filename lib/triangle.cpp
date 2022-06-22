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

#include "primitive.hpp"
#include "util.hpp"
#include "vec3.hpp"

namespace path_tracer {

Triangle::Triangle(const Vec3 &v0_a, const Vec3 &v1_a, const Vec3 &v2_a,
                   const float normal_a) {
  v0 = v0_a;
  edge1 = v1_a - v0_a;
  edge2 = v2_a - v0_a;
  normal = edge1.cross(edge2).normalize() * normal_a;
}

Triangle::Triangle(const object &obj) {
  const auto norm = get<float>(obj, "normal", "primitive");
  const auto vertices =
      get<std::array<std::array<float, 3>, 3>>(obj, "vertices", "primitive");

  const auto v1 = Vec3(vertices[1]);
  const auto v2 = Vec3(vertices[2]);

  v0 = Vec3(vertices[0]);
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

  if (t > EPSILON && t < 1.0 / EPSILON && t > t_min && t < t_max) {
    const auto point = r.origin() + r.direction() * t;
    return {{
        point,
        normal,
        t,
    }};
  }

  return std::nullopt;
}

} // namespace path_tracer
