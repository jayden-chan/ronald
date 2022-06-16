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

#include <boost/lockfree/queue.hpp>
#include <thread>

namespace path_tracer {

// 15 bounces should be more than enough for most scenes
constexpr size_t MAX_RECURSIVE_DEPTH = 15;

const Object object_from_json(const object &obj,
                              const material_map &materials) {

  const auto material_key = value_to<std::string>(obj.at("material"));
  const auto material = materials.at(material_key);

  return {
      .primitive = Primitive::from_json(obj.at("primitive").as_object()),
      .material = material,
  };
}

material_map materials_from_json(const object &obj) {
  material_map ret;
  ret.reserve(obj.size());

  for (const auto &jv : obj) {
    const std::string key = jv.key();
    ret.insert({key, Material::from_json(jv.value().as_object())});
  }

  return ret;
}

std::optional<Hit> hit_objects(const std::vector<Object> &objs,
                               const Ray &ray) {
  const auto f32_max = std::numeric_limits<float>::max();
  auto min_so_far = f32_max;
  std::optional<Hit> hit = {{
      .hit = {Vec3::zeros(), Vec3::zeros(), 0.0},
      .scatter = std::nullopt,
      .emitted = Vec3::zeros(),
  }};

  for (const auto &o : objs) {
    const auto this_hit = o.primitive->hit(ray, 0.000005F, min_so_far);
    if (this_hit.has_value()) {
      hit->hit = *this_hit;
      hit->scatter = o.material->scatter(ray, *this_hit);
      hit->emitted = o.material->emitted(ray, *this_hit);
      // const Hit new_hit = {
      //     .hit = *this_hit,
      //     .scatter = o.material->scatter(ray, *this_hit),
      //     .emitted = o.material->emitted(ray, *this_hit),
      // };
      //
      // *hit = new_hit;

      assert(this_hit->t < min_so_far);
      min_so_far = this_hit->t;
    }
  }

  if (min_so_far < f32_max) {
    return hit;
  }

  return std::nullopt;
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

Image Scene::render_single_threaded(const Config &config) const {
  const auto width = config.width;
  const auto height = config.height;
  const auto widthf = static_cast<float>(config.width - 1);
  const auto heightf = static_cast<float>(config.height - 1);
  const auto samplesf = static_cast<float>(config.samples);
  auto img = Image(width, height, ReinhardJodie);

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      auto curr_pixel = Vec3::zeros();
      for (size_t i = 0; i < config.samples; ++i) {
        const auto xf = static_cast<float>(x);
        const auto yf = static_cast<float>(height - 1 - y);
        const auto u = (xf + random_float()) / widthf;
        const auto v = (yf + random_float()) / heightf;
        curr_pixel += trace(u, v);
      }

      curr_pixel /= samplesf;
      img.set_pixel(x, y, curr_pixel);
    }
  }

  return img;
}

Image Scene::render_multi_threaded(const Config &config) const {
  const auto width = config.width;
  const auto height = config.height;
  const auto samples = config.samples;
  const auto num_threads = config.threads;
  const auto widthf = static_cast<float>(config.width - 1);
  const auto heightf = static_cast<float>(config.height - 1);
  const auto samplesf = static_cast<float>(config.samples);

  auto img = Image(width, height, ReinhardJodie);

  boost::lockfree::queue<size_t> rows(num_threads);
  rows.reserve(height);
  for (size_t i = 0; i < height; ++i) {
    rows.push(i);
  }

  // the code our threads will execute
  const auto thread_func = [&]() {
    size_t row_to_render = 0;
    for (;;) {
      const auto did_pop = rows.pop(row_to_render);

      // No more rows to render -- terminate the thread
      if (!did_pop) {
        break;
      }

      const auto y = row_to_render;

      // We did get a row to render -- go ahead and render it
      // THREAD SAFETY: geometry and material info from `this` is being
      // concurrently read by all threads. This is fine since it's all readonly
      //
      // `img` from the function scope is being concurrently written to by all
      // threads. This is also fine since the write portions of the array will
      // be non-overlapping
      for (size_t x = 0; x < width; ++x) {
        auto curr_pixel = Vec3::zeros();
        for (size_t i = 0; i < samples; ++i) {
          const auto xf = static_cast<float>(x);
          const auto yf = static_cast<float>(height - 1 - y);
          const auto u = (xf + random_float()) / widthf;
          const auto v = (yf + random_float()) / heightf;
          curr_pixel += trace(u, v);
        }

        curr_pixel /= samplesf;
        img.set_pixel(x, y, curr_pixel);
      }
    }
  };

  // start up our threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (size_t i = 0; i < num_threads; ++i) {
    threads.emplace_back(std::thread(thread_func));
  }

  // wait for completion
  for (auto &thread : threads) {
    thread.join();
  }

  return img;
}

} // namespace path_tracer
