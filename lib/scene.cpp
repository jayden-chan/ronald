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

namespace path_tracer {

// 15 bounces should be MORE than enough for most scenes
constexpr size_t MAX_RECURSIVE_DEPTH = 15;

// TODO: Change this to `Hit` and change `Hit` to `Intersection`
struct Hit {
  Intersection hit;
  std::optional<Scatter> scatter;
  Vec3 emitted;
};

std::optional<Hit> hit_objects(const std::vector<Object> &objs,
                               const Ray &ray) {
  auto min_so_far = std::numeric_limits<float>::max();
  std::optional<Hit> hit = std::nullopt;

  for (const auto o : objs) {
    const auto this_hit = o.primitive->hit(ray, 0.000005F, min_so_far);
    if (this_hit.has_value()) {
      hit = {
          .hit = *this_hit,
          .scatter = o.material->scatter(ray, *this_hit),
          .emitted = o.material->emitted(ray, *this_hit),
      };
      min_so_far = this_hit->t;
    }
  }

  return hit;
}

Vec3 Scene::trace(const float u, const float v) const {
  // TODO: Russian Roulette loop termination
  auto curr_ray = this->camera.get_ray(u, v);
  auto curr_att = Vec3::ones();
  auto total_emitted = Vec3::zeros();

  for (size_t i = 0; i < MAX_RECURSIVE_DEPTH; ++i) {
    const auto hit_result = hit_objects(this->objects, curr_ray);

    // Ray did not hit anything -- return zero
    if (!hit_result.has_value()) {
      return Vec3::zeros();
    }

    // Ray hit something but didn't scatter another ray -- path stops here
    if (!hit_result->scatter.has_value()) {
      return curr_att * (total_emitted + hit_result->emitted);
    }

    // Ray hit something and scattered, continue tracing
    curr_ray = hit_result->scatter->specular;
    curr_att *= hit_result->scatter->attenuation;
    total_emitted += hit_result->emitted;
  }

  // Max recursion depth reached -- return zero
  return Vec3::zeros();
}

Image Scene::render(const Config &config) const {
  const auto width = config.width;
  const auto height = config.height;
  auto img = Image(width, height);

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      auto curr_pixel = Vec3::zeros();
      for (size_t i = 0; i < config.samples; i++) {
        const auto u = ((float)x + random_float()) / (float)width;
        const auto v =
            ((float)(height - 1 - y) + random_float()) / (float)height;
        curr_pixel += this->trace(u, v);
      }

      curr_pixel /= (float)config.samples;
      const auto pixel = curr_pixel.to_pixel();
      img.set_pixel(x, y, pixel);
    }
  }

  return img;
}

} // namespace path_tracer
