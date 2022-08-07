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

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "aabb.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <boost/json.hpp>
using namespace boost::json;

namespace ronald {

struct Intersection {
  Vec3 point;
  Vec3 normal;
  float t;
};

/**
 * The primitive class represents any fundamental piece of geometry that a ray
 * may intersect. The Primitive class specifies one required method, hit, which
 * can test whether a given ray intersects with the geometry.
 */
class Primitive {
public:
  virtual ~Primitive() = default;

  /**
   * Check whether the given ray intersects with the primitive at some
   * point along the ray between `t_min` and `t_max`. If the ray and primitive
   * do not intersect, std::nullopt is returned.
   */
  [[nodiscard]] virtual std::optional<Intersection>
  hit(const Ray &r, float t_min, float t_max) const = 0;

  /**
   * Fetch the AABB that encloses this primitive
   */
  [[nodiscard]] virtual AABB aabb() const = 0;

  /**
   * Construct a boxed Primitive from the given JSON value.
   */
  [[nodiscard]] static const std::shared_ptr<Primitive>
  from_json(const object &obj);
};

/**
 * Simple sphere primitive. Represented by a center point
 * and a radius
 */
class Sphere : public Primitive {
  Vec3 center;
  float radius;

public:
  /**
   * Construct a sphere from a given center point and radius
   */
  [[nodiscard]] Sphere(const Vec3 &center_a, const float radius_a);

  /**
   * Construct a sphere from a JSON object containing
   * the `center` and `radius` fields
   */
  [[nodiscard]] explicit Sphere(const object &obj);

  [[nodiscard]] std::optional<Intersection> hit(const Ray &r, float t_min,
                                                float t_max) const override;
  [[nodiscard]] virtual AABB aabb() const override;
};

/**
 * Simple triangle primitive. The triangle is represented internally as a point
 * plus two edges, and is initialized using three points in 3D space plus a
 * normal vector to indicate which surface is the "front"
 */
class Triangle : public Primitive {
  Vec3 v0;
  Vec3 v1;
  Vec3 v2;
  Vec3 normal;
  Vec3 edge1;
  Vec3 edge2;

public:
  /**
   * Construct a triangle given by three points in space and a normal. The
   * normal vector should be of length 1.
   */
  [[nodiscard]] Triangle(const Vec3 &v0_a, const Vec3 &v1_a, const Vec3 &v2_a,
                         float normal_a);

  /**
   * Construct a triangle from a JSON object containing the `vertices`, and
   * `normal` fields
   */
  [[nodiscard]] explicit Triangle(const object &obj);

  [[nodiscard]] std::optional<Intersection> hit(const Ray &r, float t_min,
                                                float t_max) const override;
  [[nodiscard]] virtual AABB aabb() const override;
};

} // namespace ronald

#endif // PRIMITIVE_H
