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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <optional>

#include "ray.hpp"
#include "vec3.hpp"

struct Hit {
  Vec3 point;
  Vec3 normal;
  float t;
};

class Primitive {
public:
  virtual std::optional<Hit> hit(const Ray &r, const float t_min,
                                 const float t_max) const = 0;
};

class Sphere : public Primitive {
  Vec3 center;
  float radius;

public:
  Sphere(const Vec3 &center, const float radius);

  virtual std::optional<Hit> hit(const Ray &r, const float t_min,
                                 const float t_max) const override;
};

class Triangle : public Primitive {
  Vec3 v0;
  Vec3 normal;
  Vec3 edge1;
  Vec3 edge2;

public:
  Triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, const Vec3 &normal);

  virtual std::optional<Hit> hit(const Ray &r, const float t_min,
                                 const float t_max) const override;
};

#endif // PRIMITIVE_H
