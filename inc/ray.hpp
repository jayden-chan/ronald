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

namespace path_tracer {

/**
 * The Ray class simply represents a 2D line in 3D space.
 */
class Ray {
  Vec3 _origin;
  Vec3 _dir;

public:
  Ray(const Vec3 origin, const Vec3 dir) : _origin(origin), _dir(dir) {}

  /**
   * Return the point in 3d space at the distance `t` along
   * the ray starting at the ray origin
   */
  Vec3 point_at_parameter(const float t) const noexcept {
    return this->_origin + this->_dir * t;
  }

  inline Vec3 origin() const noexcept { return this->_origin; }
  inline Vec3 direction() const noexcept { return this->_dir; }

  /**
   * Stream insertion operator
   */
  friend std::ostream &operator<<(std::ostream &output, const Ray &r) {
    output << "{ origin: " << r.origin() << " direction: " << r.direction()
           << " }";
    return output;
  }
};

} // namespace path_tracer

#endif // RAY_H
