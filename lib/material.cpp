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

#include "material.hpp"
#include "math.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"

namespace ronald {

const std::shared_ptr<Material> Material::from_json(const object &obj) {
  const auto type = get<std::string>(obj, "type", "material");

  if (type == "light") {
    return std::make_shared<Light>(obj);
  }

  if (type == "lambertian") {
    return std::make_shared<Lambertian>(obj);
  }

  if (type == "reflector") {
    return std::make_shared<Reflector>(obj);
  }

  if (type == "dielectric") {
    return std::make_shared<Dielectric>(obj);
  }

  throw std::runtime_error("Material must be one of: [`light`, `lambertian`, "
                           "`reflector`, `dielectric`]");
}

Lambertian::Lambertian(Vec3 _albedo) : albedo(_albedo){};
Lambertian::Lambertian(const object &obj) {
  const auto alb = get<std::array<float, 3>>(obj, "albedo", "primitive");
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
  const auto emit = get<std::array<float, 3>>(obj, "emittance", "primitive");
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
  const auto atten = get<std::array<float, 3>>(obj, "attenuation", "primitive");
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

Dielectric::Dielectric(const float _ref_idx, const Vec3 _attenuation)
    : refractive_index(_ref_idx), attenuation(_attenuation){};

Dielectric::Dielectric(const object &obj) {
  refractive_index = get<float>(obj, "refractive_index", "primitive");

  if (obj.contains("attenuation")) {
    const auto atten =
        get<std::array<float, 3>>(obj, "attenuation", "primitive");
    attenuation = Vec3(atten);
  } else {
    attenuation = Vec3::ones();
  }
}

float schlick(const float cosine, const float ref_idx) {
  const auto r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
  const auto r1 = r0 * r0;

  return r1 + (1.0f - r1) * powf(1.0f - cosine, 5.0);
}

/**
 * The Reflector material reflects rays according to the law of reflection
 */
std::optional<Scatter> Dielectric::scatter(Ray const &r,
                                           Intersection const &i) const {
  const auto ray_dir = r.direction().normalize();
  const auto is_inward = ray_dir.dot(i.normal) > 0.0;
  const auto outward_normal = is_inward ? -i.normal : i.normal;
  const auto ni_over_nt =
      is_inward ? refractive_index : 1.0f / refractive_index;

  const auto cosine =
      is_inward ? refractive_index * ray_dir.dot(i.normal) * ray_dir.inv_mag()
                : -ray_dir.dot(i.normal) * ray_dir.inv_mag();

  const auto refracted = vector_refract(ray_dir, outward_normal, ni_over_nt);

  if (refracted.has_value()) {
    const auto reflect_probability =
        refracted.has_value() ? schlick(cosine, ni_over_nt) : 1.0f;

    if (random_float() >= reflect_probability) {
      return {{
          .specular = Ray(i.point, *refracted),
          .attenuation = attenuation,
      }};
    }
  }

  const auto reflected = vector_reflect(ray_dir, i.normal);
  return {{
      .specular = Ray(i.point, reflected),
      .attenuation = Vec3::ones(),
  }};
}

} // namespace ronald
