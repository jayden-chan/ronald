#ifndef RAND_H
#define RAND_H

#include <random>

/**
 * Generate a random double.
 *
 * It seems to be questionable whether the `inline` keyword here really
 * does influence this to be inlined by the compiler. Seems like GCC and MSVC
 * are free to ignore this, but Clang does respect the original intent of
 * `inline`? Not sure.
 */
inline float random_float() {
  static thread_local std::mt19937 generator;
  std::uniform_real_distribution<float> distribution(0.0, 1.0);
  return distribution(generator);
}

#endif // RAND_H
