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

#ifndef CAMERA_H
#define CAMERA_H

#include "math.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <math.h>

struct CameraConstructor {
  Vec3 look_from;
  Vec3 look_at;
  Vec3 vup;
  float vfov;
  float aspect_r;
  float aperture;
  float focus_dist;
};

class Camera {
  Vec3 u = Vec3::zeros();
  Vec3 v = Vec3::zeros();
  Vec3 w = Vec3::zeros();
  Vec3 lower_left_corner = Vec3::zeros();
  Vec3 horizontal = Vec3::zeros();
  Vec3 vertical = Vec3::zeros();
  Vec3 origin = Vec3::zeros();
  float lens_radius = 0.0;

public:
  Camera() = default;
  /**
   * Construct a new camera with the given input parameters
   */
  Camera(const CameraConstructor &cam)
      : origin(cam.look_from), lens_radius(cam.aperture / 2) {
    auto theta = cam.vfov * (float)M_PI / 180;
    auto half_height = tan(theta / 2);
    auto half_width = cam.aspect_r * half_height;

    w = (cam.look_from - cam.look_at).normalize();
    u = cam.vup.cross(w).normalize();
    v = w.cross(u);

    lower_left_corner = cam.look_from - half_width * cam.focus_dist * u -
                        half_height * cam.focus_dist * v - cam.focus_dist * w;

    horizontal = 2 * half_width * cam.focus_dist * u;
    vertical = 2 * half_height * cam.focus_dist * v;
  }

  /**
   * TODO: Fix this for the real u and v semantics
   * Return a ray which travels from the camera origin through the
   * screenspace coordinate given by `param_u` and `param_v`
   */
  Ray get_ray(const float param_u, const float param_v) const {
    auto rd = this->lens_radius * random_in_unit_disk();
    auto offset = this->u * rd.x + this->v * rd.y;

    auto ori = this->origin + offset;
    auto dir = this->lower_left_corner + this->horizontal * param_u +
               this->vertical * param_v - this->origin - offset;
    return Ray(ori, dir);
  }
};

#endif // CAMERA_H
