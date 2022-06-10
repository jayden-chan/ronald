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

#ifndef MATERIAL_H
#define MATERIAL_H

#include "primitive.hpp"
#include "ray.hpp"
#include "vec3.hpp"

struct Scatter {
  Ray specular;
  Vec3 attenuation;
};

/**
 * The material class represents the physical surface properties of a given
 * piece of geometry. It includes one virtual method for computing a scattered
 * ray given an intersection between a ray and a piece of geometry.
 */
class Material {
public:
  /**
   * Returns the "scatter" result of a ray `r` intersecting with the geometry
   * (`h`). The scatter result is optional in case the material does not scatter
   * light (if the material is a light source, for example). The scatter result
   * includes a specular ray which is the next element in the light path, and an
   * attenuation which represents the percentage of energy lost in this bounce
   * (for example the vector [0.9, 0.9, 0.9] would represent a 10% loss of
   * energy on each of the R, G, B channels)
   */
  virtual std::optional<Scatter> scatter(Ray const &r, Hit const &h) const = 0;

  /**
   * The emitted function returns the amount of light emitted by the material
   * when the ray intersects it. It is used for light source materials.
   * Non-emitting materials will simply use the default implementation of [0, 0,
   * 0] for "no light emitted". In this case all input parameters are unused,
   * hence the __attribute__((unused)) annotation.
   */
  virtual Vec3 emitted(__attribute__((unused)) Ray const &r,
                       __attribute__((unused)) Hit const &h) const {
    return Vec3::zeros();
  };

  virtual ~Material() = default;
};

/**
 * The Lambertian material is a theoretically ideal diffuse or "matte" material.
 * A ray reflecting off a Lambertian material will scatter in a random
 * direction its energy will be attenuated by the "albedo" (color) of the
 * material.
 */
class Lambertian : public Material {
private:
  Vec3 albedo;

public:
  Lambertian(Vec3 _albedo) : albedo(_albedo){};
  std::optional<Scatter> scatter(Ray const &r, Hit const &h) const override;
};

/**
 * The light material is a simple material which emits a constant amount of R,
 * G, and B light.
 */
class Light : public Material {
private:
  Vec3 emittance;

public:
  Light(Vec3 _emittance) : emittance(_emittance){};
  std::optional<Scatter> scatter(Ray const &r, Hit const &h) const override;
  Vec3 emitted(Ray const &r, Hit const &h) const override;
};

#endif // MATERIAL_H
