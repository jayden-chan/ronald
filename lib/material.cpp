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

#include "material.hpp"
#include "math.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include "vec3.hpp"

namespace path_tracer {

std::optional<Scatter> Lambertian::scatter(__attribute__((unused)) Ray const &r,
                                           Intersection const &h) const {
  const auto scattered = h.point + h.normal + random_on_unit_sphere();
  const Ray specular(h.point, scattered - h.point);
  return std::optional<Scatter>{{specular, albedo}};
}

/**
 * The light material doesn't scatter light
 */
std::optional<Scatter> Light::scatter(__attribute__((unused)) Ray const &r,
                                      __attribute__((unused))
                                      Intersection const &h) const {
  return std::nullopt;
}

Vec3 Light::emitted(Ray const &r, Intersection const &h) const {
  if (h.normal.dot(r.direction()) < 0.0) {
    return this->emittance;
  }

  return Vec3::zeros();
}

} // namespace path_tracer
