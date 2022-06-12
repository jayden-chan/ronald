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

#ifndef VEC3_H
#define VEC3_H

#include <bit>
#include <cstdint>
#include <iostream>
#include <limits>

#include "image.hpp"
#include "rand.hpp"

namespace path_tracer {

/**
 * Couldn't resist the opportunity to use this historic piece of code
 *
 * Computes the inverse square root of the input number
 *
 * https://en.wikipedia.org/wiki/Fast_inverse_square_root
 */
constexpr float Q_rsqrt(const float number) noexcept {
  static_assert(
      std::numeric_limits<float>::is_iec559); // (enable only on IEEE 754)

  float const y = std::bit_cast<float>(
      0x5f3759df - (std::bit_cast<std::uint32_t>(number) >> 1));
  return y * (1.5F - (number * 0.5F * y * y));
}

class Vec3 {
public:
  float x, y, z;

  Vec3() = default;
  Vec3(const float x_a, const float y_a, const float z_a)
      : x(x_a), y(y_a), z(z_a) {}

  /**
   * Vector of just zeros
   */
  static Vec3 zeros() noexcept { return Vec3(0.0, 0.0, 0.0); }

  /**
   * Vector of just ones
   */
  static Vec3 ones() noexcept { return Vec3(1.0, 1.0, 1.0); }

  /**
   * Vector of random values from 0 to 1
   */
  static Vec3 rand() {
    return Vec3(random_float(), random_float(), random_float());
  }

  /**
   * Return a vector with the same direction but with length 1
   */
  Vec3 normalize() const noexcept {
    auto mag_inv = Q_rsqrt((x * x) + (y * y) + (z * z));
    return Vec3(x * mag_inv, y * mag_inv, z * mag_inv);
  }

  /**
   * 1 / sqrt(mag(vector))
   */
  inline float inv_mag() const noexcept {
    return Q_rsqrt((x * x) + (y * y) + (z * z));
  }

  /**
   * Length of the vector
   */
  inline float length() const noexcept { return sqrtf(this->length_squared()); }

  /**
   * Length squared (faster than length since it avoids the sqrt)
   */
  inline float length_squared() const noexcept { return x * x + y * y + z * z; }

  /**
   * dot product of two vectors
   */
  inline float dot(const Vec3 &rhs) const noexcept {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  /**
   * cross product of two vectors
   */
  inline Vec3 cross(const Vec3 &rhs) const noexcept {
    return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x);
  }

  /**
   * Add another vector to this vector
   */
  Vec3 &operator+=(const Vec3 &v) noexcept {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  /**
   * Multiply all vector elements by a constant
   */
  Vec3 &operator*=(const float t) noexcept {
    x *= t;
    y *= t;
    z *= t;
    return *this;
  }

  /**
   * Multiply-assign vector element-wise between this vector and rhs
   */
  Vec3 &operator*=(const Vec3 v) noexcept {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  /**
   * Divide all vector elements by a constant
   */
  Vec3 &operator/=(const float t) { return *this *= 1 / t; }

  /**
   * Stream insertion operator
   */
  friend std::ostream &operator<<(std::ostream &output, const Vec3 &V) {
    output << "[ " << V.x << " " << V.y << " " << V.z << " ]";
    return output;
  }

  /**
   * Convert the vector to a pixel
   */
  Pixel to_pixel() {
    return {
        .r = this->x,
        .g = this->y,
        .b = this->z,
    };
  }
};

/********************************************************/
/*           Misc operator overloads for Vec3           */
/********************************************************/

/**
 * Add two vectors together
 */
inline Vec3 operator+(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

/**
 * Subtract one vector from another
 */
inline Vec3 operator-(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

/**
 * Multiply two vectors element-wise
 */
inline Vec3 operator*(const Vec3 &u, const Vec3 &v) noexcept {
  return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

/**
 * Multiply a vector by a constant element-wise
 */
inline Vec3 operator*(const float t, const Vec3 &v) noexcept {
  return Vec3(t * v.x, t * v.y, t * v.z);
}

/**
 * Multiply a vector by a constant element-wise
 */
inline Vec3 operator*(const Vec3 &v, const float t) noexcept { return t * v; }

/**
 * Divide a vector by a constant
 */
inline Vec3 operator/(const Vec3 v, const float t) { return (1 / t) * v; }

} // namespace path_tracer

#endif // VEC3_H
