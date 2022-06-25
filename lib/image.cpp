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

#include "image.hpp"
#include "tone.hpp"

namespace ronald {

constexpr float EIGHT_BIT_MAX_F = 255.99F;

void Image::test() {
  const auto w = this->width;
  const auto h = this->height;

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j++) {
      float r = (float)(j) / (float)(w);
      float g = (float)(i) / (float)(h);
      float b = 0;
      this->buffer.push_back({r, g, b});
    }
  }
}

void Image::write(const std::string &path) const {
  const auto w = this->width;
  const auto h = this->height;

  std::ofstream file;
  file.open(path);

  // We will use the Binary Portable PixMap (P6) image format
  // for simplicity.
  file << "P6\n";
  file << w << " " << h << '\n';
  file << "255\n";

  for (const auto p : this->buffer) {
    const auto r = (sqrt(p.x()) * EIGHT_BIT_MAX_F);
    const auto g = (sqrt(p.y()) * EIGHT_BIT_MAX_F);
    const auto b = (sqrt(p.z()) * EIGHT_BIT_MAX_F);
    assert(r >= 0 && r <= EIGHT_BIT_MAX_F);
    assert(g >= 0 && g <= EIGHT_BIT_MAX_F);
    assert(b >= 0 && b <= EIGHT_BIT_MAX_F);
    file << (unsigned char)r << (unsigned char)g << (unsigned char)b;
  }

  file.close();
}

void Image::set_pixel(const std::size_t u, const std::size_t v,
                      const Pixel &pixel) {
  this->buffer[v * this->width + u] = pixel;
}

void Image::apply_tmo() {
  switch (this->tmo) {
  case ToneMappingOperator::Clamp:
    for (auto &pixel : this->buffer) {
      tmo_clamp(pixel);
    }
    break;
  case ToneMappingOperator::ReinhardJodie:
    for (auto &pixel : this->buffer) {
      tmo_reinhard_jodie(pixel);
    }
    break;
  };
}

} // namespace ronald
