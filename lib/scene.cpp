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

#include "scene.hpp"
#include "material.hpp"
#include "vec3.hpp"

// 15 bounces should be MORE than enough for most scenes
#define MAX_RECURSIVE_DEPTH 15

// TODO: Change this to `Hit` and change `Hit` to `Intersection`
struct HitRecord {
  Hit hit;
  std::optional<Scatter> scatter;
  Vec3 emitted;
};

std::optional<HitRecord> hit_objects(const std::vector<Object> &objs,
                                     const Ray &ray) {
  auto min_so_far = std::numeric_limits<float>::max();
  std::optional<HitRecord> hit = std::nullopt;

  for (const auto o : objs) {
    const auto this_hit = o.primitive->hit(ray, 0.000005F, min_so_far);
    if (this_hit.has_value()) {
      hit = {
          *this_hit,
          o.material->scatter(ray, *this_hit),
          o.material->emitted(ray, *this_hit),
      };
      min_so_far = this_hit->t;
    }
  }

  return hit;
}

Vec3 Scene::trace(const size_t u, const size_t v) {
  // TODO: Russian Roulette loop termination
  const auto ray = this->camera.get_ray(u, v);
  auto curr_ray = ray;
  auto curr_att = Vec3::ones();
  auto total_emitted = Vec3::zeros();

  for (int i = 0; i < MAX_RECURSIVE_DEPTH; ++i) {
    const auto hit_result = hit_objects(this->objects, curr_ray);

    // Ray did not hit anything -- return zero
    if (!hit_result.has_value()) {
      return Vec3::zeros();
    } else {
      if (!hit_result->scatter.has_value()) {
        // Ray hit something but didn't scatter another ray -- path stops here
        return curr_att * (total_emitted + hit_result->emitted);
      } else {
        // Ray hit something and scattered, continue tracing
        curr_ray = hit_result->scatter->specular;
        curr_att *= hit_result->scatter->attenuation;
        total_emitted += hit_result->emitted;
      }
    }
  }

  // Max recursion depth reached -- return zero
  return Vec3::zeros();
}
