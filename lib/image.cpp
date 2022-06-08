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

#include "image.hpp"

constexpr float EIGHT_BIT_MAX_F = 255.99F;

/**
 * Test image which renders a red gradient along the X axis
 * and a green gradient along the Y axis.
 */
void Image::test() {
  auto w = this->width;
  auto h = this->height;

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      float r = (float)(j) / (float)(w);
      float g = (float)(i) / (float)(h);
      float b = 0;
      this->buffer.push_back({r, g, b});
    }
  }
}

/**
 * Write the image buffer to the provided file
 */
void Image::write(const std::string &path) const {
  auto w = this->width;
  auto h = this->height;

  std::ofstream file;
  file.open(path);

  // We will use the Binary Portable PixMap (P6) image format
  // for simplicity.
  file << "P6\n";
  file << w << " " << h << '\n';
  file << "255\n";

  for (const auto p : this->buffer) {
    file << (unsigned char)(sqrt(p.r) * EIGHT_BIT_MAX_F);
    file << (unsigned char)(sqrt(p.g) * EIGHT_BIT_MAX_F);
    file << (unsigned char)(sqrt(p.b) * EIGHT_BIT_MAX_F);
  }

  file.close();
}

/**
 * Set the pixel at the screenspace coordinate (u, v) to `pixel`
 */
void Image::set_pixel(const std::size_t u, const std::size_t v,
                      const Pixel pixel) {
  this->buffer[u * this->width + v] = pixel;
}
