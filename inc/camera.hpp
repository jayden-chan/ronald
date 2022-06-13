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
#include <stdexcept>

#include <boost/json.hpp>
using namespace boost::json;

namespace path_tracer {

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

  void init(const CameraConstructor &cam) {
    if (abs(cam.focus_dist) < EPSILON) {
      throw std::runtime_error("Focal distance cannot be zero!");
    }

    auto theta = cam.vfov * (float)M_PI / 180;
    auto half_height = tan(theta / 2);
    auto half_width = cam.aspect_r * half_height;

    origin = cam.look_from;
    lens_radius = cam.aperture / 2;
    w = (cam.look_from - cam.look_at).normalize();
    u = cam.vup.cross(w).normalize();
    v = w.cross(u);

    lower_left_corner = cam.look_from - half_width * cam.focus_dist * u -
                        half_height * cam.focus_dist * v - cam.focus_dist * w;

    horizontal = 2 * half_width * cam.focus_dist * u;
    vertical = 2 * half_height * cam.focus_dist * v;
  }

public:
  Camera() = default;
  /**
   * Construct a new camera with the given input parameters
   */
  Camera(const CameraConstructor &cam) { init(cam); }

  /**
   * Construct a camera from a JSON object containing the relevant fields for
   * camera initialization
   */
  Camera(const object &obj) {
    const auto look_from_vec =
        value_to<std::vector<float>>(obj.at("look_from"));
    const auto look_at_vec = value_to<std::vector<float>>(obj.at("look_at"));
    const auto vup_vec = value_to<std::vector<float>>(obj.at("vup"));
    const auto vfov_f = value_to<float>(obj.at("vfov"));
    const auto aspect_r_f = value_to<float>(obj.at("aspect_r"));
    const auto aperture_f = value_to<float>(obj.at("aperture"));
    const auto focus_dist_f = value_to<float>(obj.at("focus_dist"));

    const CameraConstructor cam_constructor = {.look_from = Vec3(look_from_vec),
                                               .look_at = Vec3(look_at_vec),
                                               .vup = Vec3(vup_vec),
                                               .vfov = vfov_f,
                                               .aspect_r = aspect_r_f,
                                               .aperture = aperture_f,
                                               .focus_dist = focus_dist_f};

    init(cam_constructor);
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

} // namespace path_tracer

#endif // CAMERA_H
