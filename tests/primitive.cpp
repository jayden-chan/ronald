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

  auto ray_origin = Vec3(0, 0, -10);
  auto to_origin = (Vec3(0, 0, 0) - ray_origin).normalize();
  auto ray = Ray(ray_origin, to_origin);
  REQUIRE(triangle.hit(ray, 0, 1000000000));

  ray_origin = Vec3(100, 10, -10);
  to_origin = (Vec3(0, 0, 0) - ray_origin).normalize();
  ray = Ray(ray_origin, to_origin);
  REQUIRE(triangle.hit(ray, 0, 1000000000));
}
