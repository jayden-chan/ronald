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

#ifndef MATH_H
#define MATH_H

#include "vec3.hpp"

namespace path_tracer {

constexpr float EPSILON = 0.00000001F;

/**
 * Produce a random 2D vector inside of a disk with
 * radius 1.
 */
Vec3 random_in_unit_disk();

/**
 * Produce a random 3D vector inside of a sphere with
 * radius 1
 */
Vec3 random_in_unit_sphere();

/**
 * Produce a random 3D vector which lies on the surface
 * of a sphere with radius 1
 */
Vec3 random_on_unit_sphere();

} // namespace path_tracer

#endif // MATH_H
