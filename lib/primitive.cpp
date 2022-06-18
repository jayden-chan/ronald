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

#include <optional>

#include "math.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"

namespace path_tracer {

const std::shared_ptr<Primitive> Primitive::from_json(const object &obj) {
  const auto type = get<std::string>(obj, "type", "primitive");

  if (type == "triangle") {
    return std::make_shared<Triangle>(obj);
  }

  if (type == "sphere") {
    return std::make_shared<Sphere>(obj);
  }

  throw std::runtime_error("Primitive must be either `triangle` or `sphere`");
}

Sphere::Sphere(const Vec3 &center_a, const float radius_a)
    : center(center_a), radius(radius_a) {}

Sphere::Sphere(const object &obj) {
  const auto rad = get<float>(obj, "radius", "primitive");
  const auto center_vec = get<std::array<float, 3>>(obj, "origin", "primitive");
  center = Vec3(center_vec);
  radius = rad;
}

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

std::optional<Intersection> Sphere::hit(const Ray &r, const float t_min,
                                        const float t_max) const {
  const auto oc = r.origin() - this->center;

  const auto a = r.direction().dot(r.direction());
  const auto b = oc.dot(r.direction());
  const auto c = oc.dot(oc) - this->radius * this->radius;
  const auto discriminant = b * b - a * c;

  if (discriminant > 0.0) {
    auto q_eq = (-b - sqrtf(discriminant)) / a;

    // If the minus variant is out of range try the plus one
    if (q_eq >= t_max || q_eq <= t_min) {
      q_eq = (-b + sqrtf(discriminant)) / a;
    }

    if (q_eq < t_max && q_eq > t_min) {
      const auto point_at_parameter = r.point_at_parameter(q_eq);
      return {{
          point_at_parameter,
          (point_at_parameter - this->center) / this->radius,
          q_eq,
      }};
    }
  }

  return std::nullopt;
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
