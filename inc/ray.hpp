/*
 * copyright © 2022 jayden chan. all rights reserved.
 *
 * path_tracer is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license version 3
 * as published by the free software foundation.
 *
 * path_tracer is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose. see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with path_tracer. if not, see <https://www.gnu.org/licenses/>.
 */

#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class Ray {
  Vec3 origin;
  Vec3 dir;

public:
  Ray(const Vec3 origin, const Vec3 dir) {
    this->origin = origin;
    this->dir = dir;
  }

  Vec3 point_at_parameter(const float t) const {
    return this->origin + this->dir * t;
  }
};

#endif // RAY_H
