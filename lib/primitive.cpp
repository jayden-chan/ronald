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
#include "vec3.hpp"

#include <boost/json.hpp>
using namespace boost::json;

namespace path_tracer {

const Primitive *Primitive::from_json(const object &obj) {
  const auto type = value_to<std::string>(obj.at("type"));

  if (type == "triangle") {
    return new Triangle(obj);
  }
  if (type == "sphere") {
    return new Sphere(obj);
  }

  throw std::runtime_error("Primitive must be either `triangle` or `sphere`");
}

Sphere::Sphere(const Vec3 &center_a, const float radius_a)
    : center(center_a), radius(radius_a) {}

Sphere::Sphere(const object &obj) {
  const auto rad = value_to<float>(obj.at("radius"));
  const auto center_vec = value_to<std::vector<float>>(obj.at("origin"));
  Sphere(Vec3(center_vec), rad);
}

Triangle::Triangle(const Vec3 &v0_a, const Vec3 &v1_a, const Vec3 &v2_a,
                   const Vec3 &normal_a)
    : v0(v0_a), normal(normal_a), edge1(v1_a - v0_a), edge2(v2_a - v0_a) {}

Triangle::Triangle(const object &obj) {
  const auto norm = value_to<std::vector<float>>(obj.at("normal"));
  // normal = edge1.cross(edge2).normalize() * norm;
  const auto vertices =
      value_to<std::vector<std::vector<float>>>(obj.at("vertices"));

  assert(vertices.size() == 3);
  const auto v0_ = Vec3(vertices[0]);
  const auto v1_ = Vec3(vertices[1]);
  const auto v2_ = Vec3(vertices[2]);
  Triangle(v0_, v1_, v2_, norm);
}

std::optional<Intersection> Sphere::hit(const Ray &r, const float t_min,
                                        const float t_max) const {
  Vec3 oc = r.origin() - this->center;
  auto a = r.direction().length_squared();
  auto half_b = oc.dot(r.direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0) {
    return std::nullopt;
  }

  auto sqrtd = sqrtf(discriminant);

  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || root > t_max) {
    root = (-half_b + sqrtd) / a;

    if (root < t_min || root > t_max) {
      return std::nullopt;
    }
  }

  auto t = root;
  auto point = r.point_at_parameter(t);
  auto normal = (point - center) / radius;
  return std::optional<Intersection>{{point, normal, t}};
}

/**
 * Möller–Trumbore algorithm for triangle intersection
 * https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
 */
std::optional<Intersection> Triangle::hit(const Ray &r, const float t_min,
                                          const float t_max) const {
  auto h = r.direction().cross(edge2);
  auto a = edge1.dot(h);

  if (a > -EPSILON && a < EPSILON) {
    return std::nullopt;
  }

  auto f = 1 / a;
  auto s = r.origin() - v0;
  auto u = f * s.dot(h);

  // this branch might be unpredictable, should check this
  if (u < 0.0 || u > 1.0) {
    return std::nullopt;
  }

  auto q = s.cross(edge1);
  auto v = f * r.direction().dot(q);

  // this might also be unpredictable, check it
  if (v < 0.0 || u + v > 1.0) {
    return std::nullopt;
  }

  auto t = f * edge2.dot(q);
  auto point = r.origin() + r.direction() * t;

  if (t > EPSILON && t < 1.0 / EPSILON && t > t_min && t < t_max) {
    return std::optional<Intersection>{{
        point,
        normal,
        t,
    }};
  }

  return std::nullopt;
}

} // namespace path_tracer
