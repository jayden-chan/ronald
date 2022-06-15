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

  if (type == "reflector") {
    return new Reflector(obj);
  }

  if (type == "dielectric") {
    return new Dielectric(obj);
  }

  throw std::runtime_error("Material must be one of: [`light`, `lambertian`, "
                           "`reflector`, `dielectric`]");
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

Reflector::Reflector(Vec3 _attenuation) : attenuation(_attenuation){};
Reflector::Reflector(const object &obj) {
  const auto atten = value_to<std::vector<float>>(obj.at("attenuation"));
  attenuation = Vec3(atten);
}

/**
 * The Reflector material reflects rays according to the law of reflection
 */
std::optional<Scatter> Reflector::scatter(Ray const &r,
                                          Intersection const &h) const {
  const auto reflected = vector_reflect(r.direction().normalize(), h.normal);
  const auto specular = Ray(h.point, reflected);

  if (specular.direction().dot(h.normal) > 0.0) {
    return {{specular, attenuation}};
  } else {
    return std::nullopt;
  }
}

Dielectric::Dielectric(const float _ref_idx) : refractive_index(_ref_idx){};
Dielectric::Dielectric(const object &obj) {
  refractive_index = value_to<float>(obj.at("refractive_index"));
}

float schlick(const float cosine, const float ref_idx) {
  auto r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
  r0 = r0 * r0;

  return r0 + (1.0f - r0) * powf(1.0f - cosine, 5.0);
}

/**
 * The Reflector material reflects rays according to the law of reflection
 */
std::optional<Scatter> Dielectric::scatter(Ray const &r,
                                           Intersection const &i) const {
  Vec3 outward_normal;
  float ni_over_nt;
  float cosine;

  if (r.direction().dot(i.normal) > 0.0) {
    outward_normal = -i.normal;
    ni_over_nt = this->refractive_index;
    cosine = this->refractive_index * r.direction().dot(i.normal) *
             r.direction().inv_mag();
  } else {
    outward_normal = i.normal;
    ni_over_nt = 1.0f / this->refractive_index;
    cosine = -r.direction().dot(i.normal) * r.direction().inv_mag();
  };

  const auto reflected = vector_reflect(r.direction(), i.normal);
  const auto refracted =
      vector_refract(r.direction(), outward_normal, ni_over_nt);

  const auto reflect_probability =
      refracted.has_value() ? schlick(cosine, this->refractive_index) : 1.0;

  const auto dir =
      random_float() < reflect_probability ? reflected : *refracted;

  return std::optional<Scatter>({
      Ray(i.point, dir),
      Vec3::ones(),
  });
}

} // namespace path_tracer
