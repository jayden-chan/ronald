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

#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <utility>

#include "dbg.h"
#include "rand.hpp"

constexpr float EPSILON = 0.00000001F;

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
private:
  float v[3] = {0.0f, 0.0f, 0.0f};

public:
  constexpr Vec3() = default;
  /**
   * Construct a Vec3 from x, y, and z values
   */
  constexpr Vec3(const float x, const float y, const float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
  }

  /**
   * Construct a Vec3 from a std::array containing exactly
   * three floats (x, y, z in that order)
   */
  explicit Vec3(const std::array<float, 3> vals) {
    v[0] = vals.at(0);
    v[1] = vals.at(1);
    v[2] = vals.at(2);
  }

  /**
   * Vector of just zeros. The default constructor is all zeros as well but this
   * is a bit more clear in the code
   */
  static constexpr Vec3 zeros() noexcept { return Vec3(0.0, 0.0, 0.0); }

  /**
   * Vector of just ones
   */
  static constexpr Vec3 ones() noexcept { return Vec3(1.0, 1.0, 1.0); }

  /**
   * Vector of random values from 0 to 1
   */
  static Vec3 rand() {
    return Vec3(random_float(), random_float(), random_float());
  }

  /**
   * Access the x value
   */
  [[nodiscard]] inline float x() const noexcept { return v[0]; }

  /**
   * Access the y value
   */
  [[nodiscard]] inline float y() const { return v[1]; }

  /**
   * Access the z value
   */
  [[nodiscard]] inline float z() const { return v[2]; }

  /**
   * Set the x value
   */
  inline void set_x(const float val) noexcept { v[0] = val; }

  /**
   * Set the y value
   */
  inline void set_y(const float val) noexcept { v[1] = val; }

  /**
   * Set the z value
   */
  inline void set_z(const float val) noexcept { v[2] = val; }

  /**
   * Return a vector with the same direction but with length 1
   */
  [[nodiscard]] inline Vec3 normalize() const {
    const auto mag_inv = this->inv_mag();
    return Vec3(v[0] * mag_inv, v[1] * mag_inv, v[2] * mag_inv);
  }

  /**
   * 1 / sqrt(mag(vector))
   */
  [[nodiscard]] inline float inv_mag() const {
    return 1.0f / sqrtf(this->length_squared());
  }

  /**
   * Length of the vector
   */
  [[nodiscard]] inline float length() const noexcept {
    return sqrtf(this->length_squared());
  }

  /**
   * Length squared (faster than length since it avoids the sqrt)
   */
  [[nodiscard]] inline float length_squared() const noexcept {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  }

  /**
   * dot product of two vectors
   */
  [[nodiscard]] inline float dot(const Vec3 &rhs) const noexcept {
    return v[0] * rhs.x() + v[1] * rhs.y() + v[2] * rhs.z();
  }

  /**
   * cross product of two vectors
   */
  [[nodiscard]] inline Vec3 cross(const Vec3 &rhs) const noexcept {
    return Vec3(v[1] * rhs.z() - v[2] * rhs.y(),
                v[2] * rhs.x() - v[0] * rhs.z(),
                v[0] * rhs.y() - v[1] * rhs.x());
  }

  /**
   * Add another vector to this vector
   */
  Vec3 &operator+=(const Vec3 &rhs) noexcept {
    v[0] += rhs.x();
    v[1] += rhs.y();
    v[2] += rhs.z();
    return *this;
  }

  /**
   * Multiply all vector elements by a constant
   */
  Vec3 &operator*=(const float t) noexcept {
    v[0] *= t;
    v[1] *= t;
    v[2] *= t;
    return *this;
  }

  /**
   * Multiply-assign vector element-wise between this vector and rhs
   */
  Vec3 &operator*=(const Vec3 rhs) noexcept {
    v[0] *= rhs.x();
    v[1] *= rhs.y();
    v[2] *= rhs.z();
    return *this;
  }

  /**
   * Divide all vector elements by a constant
   */
  Vec3 &operator/=(const float t) { return *this *= 1 / t; }

  /**
   * Negate all elements of the vector
   */
  [[nodiscard]] Vec3 operator-() const { return Vec3(-v[0], -v[1], -v[2]); }

  /**
   * Access the elements of the vector by numeric index. 0=x, 1=y, 2=z.
   * Any index less than zero or greater than 2 is undefined behavior
   */
  float operator[](const size_t idx) const {
    assert(idx < 3);
    return v[idx];
  }

  /**
   * Stream insertion operator
   */
  friend std::ostream &operator<<(std::ostream &output, const Vec3 &V) {
    output << "[ " << V.x() << " " << V.y() << " " << V.z() << " ]";
    return output;
  }
};

/********************************************************/
/*           Misc operator overloads for Vec3           */
/********************************************************/

/**
 * Add two vectors together
 */
[[nodiscard]] inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return Vec3(lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z());
}

/**
 * Add a constant to all elements of a vector
 */
[[nodiscard]] inline Vec3 operator+(const Vec3 &lhs,
                                    const float &rhs) noexcept {
  return Vec3(lhs.x() + rhs, lhs.y() + rhs, lhs.z() + rhs);
}

/**
 * Subtract one vector from another
 */
[[nodiscard]] inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return Vec3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
}

/**
 * Multiply two vectors element-wise
 */
[[nodiscard]] inline Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs) noexcept {
  return Vec3(lhs.x() * rhs.x(), lhs.y() * rhs.y(), lhs.z() * rhs.z());
}

/**
 * Multiply a vector by a constant element-wise
 */
[[nodiscard]] inline Vec3 operator*(const float t, const Vec3 &v) noexcept {
  return Vec3(t * v.x(), t * v.y(), t * v.z());
}

/**
 * Multiply a vector by a constant element-wise
 */
[[nodiscard]] inline Vec3 operator*(const Vec3 &v, const float t) noexcept {
  return t * v;
}

/**
 * Divide a vector by a constant
 */
[[nodiscard]] inline Vec3 operator/(const Vec3 &v, const float t) {
  return (1 / t) * v;
}

/**
 * Divide two vectors element-wise
 */
[[nodiscard]] inline Vec3 operator/(const Vec3 &lhs, const Vec3 &rhs) {
  return Vec3(lhs.x() / rhs.x(), lhs.y() / rhs.y(), lhs.z() / rhs.z());
}

/**
 * Vectors will be considered equal if all of their elements are
 * within `EPSILON` of each other. This function is used for unit testing
 * only for now
 */
[[nodiscard]] inline bool operator==(const Vec3 &lhs, const Vec3 &rhs) {
  return (abs(lhs.x() - rhs.x()) < EPSILON &&
          abs(lhs.y() - rhs.y()) < EPSILON && abs(lhs.z() - rhs.z()) < EPSILON);
}

[[nodiscard]] inline Vec3 vector_reflect(const Vec3 &v, const Vec3 &n) {
  return v - 2.0f * v.dot(n) * n;
}

[[nodiscard]] inline std::optional<Vec3>
vector_refract(const Vec3 &v_a, const Vec3 &n, const float ni_over_nt) {
  const auto v = v_a.normalize();
  const auto dt = v.dot(n);
  const auto discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);

  // Ray refracts
  if (discriminant > 0.0) {
    const auto ret = ni_over_nt * (v - n * dt) - n * sqrtf(discriminant);
    return ret;
  }

  // Total internal reflection
  return std::nullopt;
}

} // namespace path_tracer

#endif // VEC3_H
