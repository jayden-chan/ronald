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
#include "ray.hpp"
#include "util.hpp"

namespace path_tracer {

Sphere::Sphere(const Vec3 &center_a, const float radius_a)
    : center(center_a), radius(radius_a) {}

Sphere::Sphere(const object &obj) {
  const auto rad = get<float>(obj, "radius", "primitive");
  const auto center_vec = get<std::array<float, 3>>(obj, "origin", "primitive");
  center = Vec3(center_vec);
  radius = rad;
}

std::optional<Intersection> Sphere::hit(const Ray &r, const float t_min,
                                        const float t_max) const {
  Vec3 oc = r.origin() - center;
  const auto a = r.direction().length_squared();
  const auto half_b = oc.dot(r.direction());
  const auto c = oc.length_squared() - radius * radius;

  const auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return std::nullopt;
  }

  const auto sqrtd = sqrtf(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root) {
      return std::nullopt;
    }
  }

  const auto point = r.point_at_parameter(root);
  return {{
      .point = point,
      .normal = (point - center) / radius,
      .t = root,
  }};
}
} // namespace path_tracer
