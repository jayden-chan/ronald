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

#ifndef SCENE_H
#define SCENE_H

#include "camera.hpp"
#include "material.hpp"
#include "primitive.hpp"
#include "vec3.hpp"

#include <stdlib.h>

/**
 * An object is a primitive with an associated material
 */
struct Object {
  Primitive *primitive;
  Material *material;
};

/**
 * The scene is composed of the objects and the camera
 */
class Scene {
private:
  // TODO: BVH
  std::vector<Object> objects;
  Camera camera;

public:
  Vec3 trace(size_t u, size_t v);
};

#endif // SCENE_H
