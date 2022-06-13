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

#include <boost/json.hpp>
using namespace boost::json;

namespace path_tracer {

const Material *Material::from_json(const object &obj) {
  const auto type = value_to<std::string>(obj.at("type"));

  if (type == "light") {
    return new Light(obj);
  }

  if (type == "lambertian") {
    return new Lambertian(obj);
  }

  throw std::runtime_error("Material must be either `light` or `lambertian`");
}

Lambertian::Lambertian(Vec3 _albedo) : albedo(_albedo){};
Lambertian::Lambertian(const object &obj) {
  const auto alb = value_to<std::vector<float>>(obj.at("albedo"));
  albedo = Vec3(alb);
}

std::optional<Scatter> Lambertian::scatter(__attribute__((unused)) Ray const &r,
                                           Intersection const &h) const {
  const auto scattered = h.point + h.normal + random_on_unit_sphere();
  const Ray specular(h.point, scattered - h.point);
  return std::optional<Scatter>{{specular, albedo}};
}

Light::Light(Vec3 _emittance) : emittance(_emittance){};
Light::Light(const object &obj) {
  const auto emit = value_to<std::vector<float>>(obj.at("emittance"));
  emittance = Vec3(emit);
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
