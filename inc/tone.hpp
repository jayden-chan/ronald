/*
 * Copyright © 2022 Jayden Chan. All rights reserved.
 *
 * Ronald is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * Ronald is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ronald. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TONE_H
#define TONE_H

/**
 * This file provides several utilities for mapping the linear
 * HDR radiance values from the renderer down into 24bit LDR
 * RGB values.
 */

#include "vec3.hpp"
#include <stdexcept>
namespace ronald {

constexpr float clamp_max = 1.0f;

/**
 * Tone-map the image by clamping each pixel
 * within the hard-coded clamp limits (1.0).
 */
inline void tmo_clamp(Vec3 &pixel) {
  if (pixel.x() > clamp_max) {
    pixel.set_x(clamp_max);
  }

  if (pixel.y() > clamp_max) {
    pixel.set_y(clamp_max);
  }

  if (pixel.z() > clamp_max) {
    pixel.set_z(clamp_max);
  }

  // This should never happen in normal circumstances unless someone
  // creates an invalid input such as a light source that has negative
  // emittance.
  if (pixel.x() < 0 || pixel.y() < 0 || pixel.z() < 0) {
    throw std::runtime_error("ERROR: Pixel value cannot be lower than zero");
  }
}

constexpr Vec3 luminance_triple = Vec3(0.2126f, 0.7152f, 0.0722f);
[[nodiscard]] inline float luminance(const Vec3 &pixel) {
  return pixel.dot(luminance_triple);
}

[[nodiscard]] inline Vec3 lerp(const Vec3 &a, const Vec3 &b, const Vec3 &t) {
  return a + t * (b - a);
}

/**
 * Reinhard-Jodie tone mapping algorithm as described here
 * https://64.github.io/tonemapping/
 *
 * This page has a bunch of great information about tone
 * mapping HDR images back down to standard 24 bit dynamic
 * range images. I would have loved to implement some more of
 * these but there just wasn't time.
 */
inline void tmo_reinhard_jodie(Vec3 &pixel) {
  const auto p_new = pixel / (pixel + 1.0f);

  pixel.set_x(p_new.x());
  pixel.set_y(p_new.y());
  pixel.set_z(p_new.z());
}

} // namespace ronald

#endif // TONE_H
