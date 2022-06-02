#include <catch2/catch.hpp>

#include "primitive.hpp"
#include "ray.hpp"
#include "vec3.hpp"

TEST_CASE("Ray hits triangle", "[primitive][ray][triangle]") {
  // Triangle at the origin on the XY plane
  auto const v0 = Vec3(0, 5, 0);
  auto const v1 = Vec3(-5, -5, 0);
  auto const v2 = Vec3(5, -5, 0);
  auto const triangle = Triangle(v0, v1, v2, Vec3(0, 0, -1));

  // send a ray through the origin and ensure it hits
  auto const ray_origin = Vec3(0, 0, -10);
  auto const to_origin = (Vec3(0, 0, 0) - ray_origin).normalize();
  auto const ray = Ray(ray_origin, to_origin);
  REQUIRE(triangle.hit(ray, 0, 1000000000));

  // generate a bunch of random rays through the origin and
  // ensure they all hit
  for (int i = 0; i < 10; i++) {
    auto const rand_point = Vec3::rand() * 1000;
    auto const dir_to_origin = (Vec3(0, 0, 0) - rand_point).normalize();
    auto const r = Ray(rand_point, dir_to_origin);
    INFO("Expecting vec to hit: " << r);
    auto const hit = triangle.hit(r, 0, 1000000000);
    REQUIRE(hit);
  }

  // generate a bunch of random rays and ensure they all miss.
  // technically there is a chance that one of these could hit but
  // it's very unlikely. probably still bad testing practice but oh well
  for (int i = 0; i < 10; i++) {
    auto const r1 = Vec3::rand() * 10000;
    auto const r2 = Vec3::rand() * 10000;
    auto const dir = (r1 - r2).normalize();
    auto const r = Ray(r1, dir);
    INFO("Expecting vec to miss: " << r);
    auto const hit = triangle.hit(r, 0, 1000000000);
    REQUIRE(!hit);
  }
}
