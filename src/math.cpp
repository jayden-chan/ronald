#include "math.hpp"

Vec3 random_in_unit_disk() {
  auto v1 = Vec3(random_float(), random_float(), 0);
  auto v2 = Vec3(1, 1, 0);
  auto p = 2 * v1 - v2;

  while (p.dot(p) >= 1) {
    v1 = Vec3(random_float(), random_float(), 0);
    v2 = Vec3(1, 1, 0);
    p = 2 * v1 - v2;
  }

  return p;
}

Vec3 random_in_unit_sphere() {
  auto p = 2 * Vec3::rand() - Vec3::ones();

  while (p.dot(p) >= 1) {
    p = 2 * Vec3::rand() - Vec3::ones();
  }

  return p;
}

Vec3 random_on_unit_sphere() { return random_in_unit_sphere().normalize(); }
