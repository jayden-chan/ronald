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
#include "image.hpp"
#include "inputs.hpp"
#include "material.hpp"
#include "primitive.hpp"
#include "vec3.hpp"

#include <stdlib.h>

#include <boost/json.hpp>
using namespace boost::json;

namespace path_tracer {

/**
 * An object is a primitive with an associated material
 */
struct Object {
  const Primitive *const primitive;
  const Material *const material;
};

/**
 * Represents an occurrence of a light ray hitting a primitive geometric object
 */
struct Hit {
  Intersection hit;
  std::optional<Scatter> scatter;
  Vec3 emitted;
};

/*
 * {
 *  "material": {
 *    "type": "light",
 *    "emittance": [0, 0, 0],
 *  },
 *  "primitive": {
 *    "type": "triangle",
 *    "vertices": ...
 *  }
 * }
 */
const Object object_from_json(const object &obj);

/**
 * The scene is composed of the objects and the camera
 */
class Scene {
private:
  // TODO: BVH
  std::vector<Object> objects;
  Camera camera;

  /**
   * Sends a ray through the scene geometry based on the given
   * screenspace coordinate (u, v) and returns the pixel's luminance
   */
  Vec3 trace(float u, float v) const;

public:
  Scene(std::vector<Object> &objects_a, Camera &camera_a)
      : objects(objects_a), camera(camera_a){};

  /**
   * Calls `trace` for each pixel in the scene for as many samples
   * as specified in the config
   */
  Image render(const Config &config) const;
};

} // namespace path_tracer

#endif // SCENE_H
