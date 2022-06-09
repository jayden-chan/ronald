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
  virtual std::optional<Scatter> scatter(Ray const &r, Hit const &h) const = 0;
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

#endif // MATERIAL_H
