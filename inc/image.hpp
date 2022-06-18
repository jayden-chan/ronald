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

#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

namespace path_tracer {

using Pixel = Vec3;

enum ToneMappingOperator {
  Clamp,
  ReinhardJodie,
};

class Image {
  std::vector<Pixel> buffer;
  size_t width = 0;
  size_t height = 0;
  ToneMappingOperator tmo = ReinhardJodie;

public:
  Image() = default;

  Image(const size_t width_a, const size_t height_a,
        const ToneMappingOperator tmo_a)
      : width(width_a), height(height_a), tmo(tmo_a) {
    this->buffer.reserve(width_a * height_a);
    this->buffer.resize(width_a * height_a);
  }

  /**
   * Test image which renders a red gradient along the X axis
   * and a green gradient along the Y axis.
   */
  void test();

  /**
   * Write the image buffer to the provided file
   */
  void write(const std::string &path) const;

  /**
   * Set the pixel at the screenspace coordinate (u, v) to `pixel`
   */
  void set_pixel(size_t u, size_t v, const Pixel &pixel);

  /**
   * Apply the given tone mapping operator to the image
   */
  void apply_tmo();
};

} // namespace path_tracer

#endif // IMAGE_H
