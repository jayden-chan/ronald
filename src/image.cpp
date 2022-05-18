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

/**
 * Test image which renders a red gradient along the X axis
 * and a green gradient along the Y axis.
 */
void Image::test() {
  auto w = this->width;
  auto h = this->height;
  this->buffer.reserve((size_t)w * (size_t)h);

  for (auto i = 0; i < height; i++) {
    for (auto j = 0; j < width; j++) {
      float r = static_cast<float>(j) / static_cast<float>(w);
      float g = static_cast<float>(i) / static_cast<float>(h);
      float b = 0;
      this->buffer.push_back({r, g, b});
    }
  }
}

/**
 * Write the image buffer to the provided file
 */
void Image::write(const std::string path) {
  auto w = this->width;
  auto h = this->height;

  std::ofstream myfile;
  myfile.open(path);

  // We will use the Binary Portable PixMap (P6) image format
  // for simplicity.
  myfile << "P6\n";
  myfile << w << " " << h << "\n";
  myfile << "255\n";

  for (const auto p : this->buffer) {
    myfile << (unsigned char)(sqrt(p.r) * 255.99);
    myfile << (unsigned char)(sqrt(p.g) * 255.99);
    myfile << (unsigned char)(sqrt(p.b) * 255.99);
  }

  myfile.close();
}
