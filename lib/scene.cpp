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
#include "progress.hpp"
#include "util.hpp"
#include "vec3.hpp"

#include <boost/lockfree/queue.hpp>
#include <thread>

using namespace std::chrono_literals;

namespace path_tracer {

// 20 bounces should be more than enough for most scenes
constexpr size_t MAX_RECURSIVE_DEPTH = 20;

const Object object_from_json(const object &obj,
                              const material_map &materials) {

  const auto material_key = get<std::string>(obj, "material", "objects");
  if (!materials.contains(material_key)) {
    throw std::runtime_error("Undefined reference to material \"" +
                             material_key + "\"");
  }

  const auto material = materials.at(material_key);
  const auto primitive =
      Primitive::from_json(at(obj, "primitive", "objects").as_object());

  return {
      .primitive = primitive,
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
  constexpr float T_MIN = 0.0005f;
  constexpr auto f32_max = std::numeric_limits<float>::max();

  auto min_so_far = f32_max;
  std::optional<Hit> hit = {{
      .hit = {Vec3::zeros(), Vec3::zeros(), 0.0},
      .scatter = std::nullopt,
      .emitted = Vec3::zeros(),
  }};

  std::optional<Intersection> last_hit = std::nullopt;
  size_t last_obj_hit = 0;

  for (size_t i = 0; i < objs.size(); ++i) {
    const auto &o = objs[i];
    const auto this_hit = o.primitive->hit(ray, T_MIN, min_so_far);
    if (this_hit.has_value()) {
      hit->hit = *this_hit;
      last_hit = this_hit;
      last_obj_hit = i;

      assert(this_hit->t < min_so_far);
      min_so_far = this_hit->t;
    }
  }

  if (min_so_far < f32_max) {
    // Delay computing the scattered and emitted results until we've figured
    // out which object we actually hit (if any)
    hit->scatter = objs[last_obj_hit].material->scatter(ray, *last_hit);
    hit->emitted = objs[last_obj_hit].material->emitted(ray, *last_hit);
    return hit;
  }

  return std::nullopt;
}

Vec3 Scene::trace(const float u, const float v) const {
  auto curr_ray = this->camera.get_ray(u, v);
  auto total_attenuation = Vec3::ones();
  auto total_emitted = Vec3::zeros();

  for (size_t i = 0; i < MAX_RECURSIVE_DEPTH; ++i) {
    const auto hit_result = hit_objects(this->objects, curr_ray);

    // Ray did not hit anything -- return zero
    if (!hit_result.has_value()) {
      return Vec3::zeros();
    }

    total_emitted += hit_result->emitted;

    // Ray hit something but didn't scatter another ray -- path stops here
    if (!hit_result->scatter.has_value()) {
      return total_attenuation * total_emitted;
    }

    // Ray hit something and scattered, continue tracing
    total_attenuation *= hit_result->scatter->attenuation;
    curr_ray = hit_result->scatter->specular;

    // Terminate the path with a probability inversely proportional to the
    // current attenuation. Paths with lower contribution to the scene are more
    // likely to be terminated. This is known as "Russian Roulette" termination.
    const auto p =
        std::max(total_attenuation.x(),
                 std::max(total_attenuation.y(), total_attenuation.z()));

    if (random_float() > p) {
      return total_attenuation * total_emitted;
    }

    // We didn't get terminated by Russian Roulette -- add back the energy
    // lost through random terminations. This step ensures that the renderer
    // remains unbiased despite terminating some paths early
    total_attenuation *= 1.0f / p;
  }

  // Max recursion depth reached -- return zero
  return Vec3::zeros();
}

std::optional<Scene> Scene::from_json(const object &obj, const float aspect_r) {
  try {
    const auto material_obj = at(obj, "materials").as_object();
    const auto materials = materials_from_json(material_obj);

    const auto json_objs = at(obj, "objects").as_array();
    std::vector<Object> objs;
    objs.reserve(json_objs.size());

    for (const auto &o : json_objs) {
      const auto o_as_obj = o.as_object();
      const auto material_key =
          get<std::string>(o_as_obj, "material", "objects");

      if (!materials.contains(material_key)) {
        throw std::runtime_error("Undefined reference to material \"" +
                                 material_key + "\"");
      }

      const auto material = materials.at(material_key);
      const auto primitives = at(o_as_obj, "primitives", "objects");

      for (const auto &p : primitives.as_array()) {
        const auto primitive = Primitive::from_json(p.as_object());
        objs.push_back({.primitive = primitive, .material = material});
      }
    }

    const auto cam = Camera(at(obj, "camera").as_object(), aspect_r);
    return Scene(objs, materials, cam);
  } catch (std::exception &e) {
    std::cout << "ERROR: Failed to parse scene from JSON: " << e.what() << '\n';
    return std::nullopt;
  }
}

Image Scene::render_single_threaded(const Config &config) const {
  const auto width = config.width;
  const auto height = config.height;
  const auto widthf = static_cast<float>(config.width - 1);
  const auto heightf = static_cast<float>(config.height - 1);
  const auto samplesf = static_cast<float>(config.samples);
  auto img = Image(width, height, ReinhardJodie);

  for (size_t y = 0; y < height; ++y) {
    const auto yf = static_cast<float>(height - 1 - y);

    for (size_t x = 0; x < width; ++x) {
      const auto xf = static_cast<float>(x);

      auto curr_pixel = Vec3::zeros();
      for (size_t i = 0; i < config.samples; ++i) {
        const auto u = (xf + random_float()) / widthf;
        const auto v = (yf + random_float()) / heightf;
        curr_pixel += trace(u, v);
      }

      curr_pixel /= samplesf;
      img.set_pixel(x, y, curr_pixel);
    }

    print_progress((float)y / heightf);
  }

  std::cout << std::endl;
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

  boost::lockfree::queue<size_t> in_progress_rows(num_threads);
  boost::lockfree::queue<size_t> completed_rows(num_threads);
  in_progress_rows.reserve(height);
  completed_rows.reserve(height);

  for (size_t i = 0; i < height; ++i) {
    in_progress_rows.push(i);
  }

  // the code our threads will execute
  const auto thread_func = [&]() {
    size_t row_to_render = 0;
    for (;;) {
      const auto did_pop = in_progress_rows.pop(row_to_render);

      // No more rows to render -- terminate the thread
      if (!did_pop) {
        break;
      }

      const auto y = row_to_render;
      const auto yf = static_cast<float>(height - 1 - y);

      // We did get a row to render -- go ahead and render it
      // THREAD SAFETY: geometry and material info from `this` is being
      // concurrently read by all threads. This is fine since it's all readonly
      //
      // `img` from the function scope is being concurrently written to by all
      // threads. This is also fine since the write portions of the array will
      // be non-overlapping
      for (size_t x = 0; x < width; ++x) {
        const auto xf = static_cast<float>(x);

        auto curr_pixel = Vec3::zeros();
        for (size_t i = 0; i < samples; ++i) {
          const auto u = (xf + random_float()) / widthf;
          const auto v = (yf + random_float()) / heightf;
          curr_pixel += trace(u, v);
        }

        curr_pixel /= samplesf;
        img.set_pixel(x, y, curr_pixel);
      }

      completed_rows.push(row_to_render);
    }
  };

  // start up our threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
  for (size_t i = 0; i < num_threads; ++i) {
    threads.emplace_back(std::thread(thread_func));
  }

  size_t rows_completed = 0;

  // There's no way to "block on" the output queue so we'll just read it
  // every 300ms
  for (;;) {
    size_t tmp;
    while (completed_rows.pop(tmp)) {
      rows_completed += 1;
    }

    print_progress((float)rows_completed / heightf);

    if (rows_completed == height) {
      break;
    }

    std::this_thread::sleep_for(300ms);
  }

  std::cout << std::endl;

  // wait for completion
  for (auto &thread : threads) {
    thread.join();
  }

  return img;
}

} // namespace path_tracer
