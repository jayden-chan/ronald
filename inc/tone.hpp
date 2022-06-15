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

#ifndef TONE_H
#define TONE_H

#include "vec3.hpp"
#include <stdexcept>
namespace path_tracer {

constexpr float clamp_max = 1.0f;

inline void tmo_clamp(Vec3 &pixel) {
  if (pixel.x > clamp_max) {
    pixel.x = clamp_max;
  }
  if (pixel.y > clamp_max) {
    pixel.y = clamp_max;
  }
  if (pixel.z > clamp_max) {
    pixel.z = clamp_max;
  }

  // This should never happen in normal circumstances unless someone
  // creates an invalid input such as a light source that has negative
  // emittance.
  if (pixel.x < 0 || pixel.y < 0 || pixel.z < 0) {
    throw std::runtime_error("ERROR: Pixel value cannot be lower than zero");
  }
}

constexpr Vec3 luminance_triple = Vec3(0.2126f, 0.7152f, 0.0722f);
inline float luminance(const Vec3 &pixel) {
  return pixel.dot(luminance_triple);
}

inline Vec3 lerp(const Vec3 &a, const Vec3 &b, const Vec3 &t) {
  return a + t * (b - a);
}

inline void tmo_reinhard_jodie(Vec3 &pixel) {
  const auto l = luminance(pixel);
  const auto tv = pixel / (pixel + 1.0f);
  pixel = lerp(pixel / (1.0f + l), tv, tv);
}

} // namespace path_tracer

#endif // TONE_H
