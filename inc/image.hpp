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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

struct Pixel {
  float r;
  float g;
  float b;
};

class Image {
  std::vector<Pixel> buffer;
  size_t width, height;

public:
  Image(const size_t width_a, const size_t height_a)
      : width(width_a), height(height_a) {
    this->buffer.reserve(width_a * height_a);
    this->buffer.resize(width_a * height_a);
  }

  void test();
  void write(const std::string &path) const;
  void set_pixel(size_t u, size_t v, Pixel pixel);
};

#endif // IMAGE_H
