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
    const auto theta = cam.vfov * (float)M_PI / 180;
    const auto h = tan(theta / 2);
    const auto viewport_h = h * 2;
    const auto viewport_w = cam.aspect_r * viewport_h;
    const auto focus_dist = (cam.look_from - cam.look_at).length();

    // TODO: this is a bit of a hack, should really diagnose the underlying
    // issue
    const auto aperture = cam.aperture < 0.005 ? 0.005f : cam.aperture;

    origin = cam.look_from;
    lens_radius = aperture / 2;
    w = (cam.look_from - cam.look_at).normalize();
    u = cam.vup.cross(w).normalize();
    v = w.cross(u);

    horizontal = viewport_w * focus_dist * u;
    vertical = viewport_h * focus_dist * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
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
  Camera(const object &obj, const float aspect_r) {
    const auto look_from_vec =
        value_to<std::vector<float>>(obj.at("look_from"));
    const auto look_at_vec = value_to<std::vector<float>>(obj.at("look_at"));
    const auto vup_vec = value_to<std::vector<float>>(obj.at("vup"));
    const auto vfov_f = value_to<float>(obj.at("vfov"));
    const auto aperture_f = value_to<float>(obj.at("aperture"));

    const CameraConstructor cam_constructor = {.look_from = Vec3(look_from_vec),
                                               .look_at = Vec3(look_at_vec),
                                               .vup = Vec3(vup_vec),
                                               .vfov = vfov_f,
                                               .aspect_r = aspect_r,
                                               .aperture = aperture_f};

    init(cam_constructor);
  }

  /**
   * TODO: Fix this for the real u and v semantics
   * Return a ray which travels from the camera origin through the
   * screenspace coordinate given by `param_u` and `param_v`
   */
  Ray get_ray(const float s, const float t) const {
    const auto rd = lens_radius * random_in_unit_disk();
    const auto offset = u * rd.x + v * rd.y;

    const auto ori = origin + offset;
    const auto dir =
        lower_left_corner + horizontal * s + vertical * t - origin - offset;

    return Ray(ori, dir);
  }
};

} // namespace path_tracer

#endif // CAMERA_H
