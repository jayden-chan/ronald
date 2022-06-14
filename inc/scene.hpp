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
#include <unordered_map>
using namespace boost::json;

namespace path_tracer {

using material_map = std::unordered_map<std::string, const Material *>;

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

/**
 * Create an object from a JSON file containing the "material"
 * and "primitive" fields
 */
const Object object_from_json(const object &obj, const material_map &materials);

/**
 * Create a list of materials from a JSON array containing material objects
 */
material_map materials_from_json(const object &obj);

/**
 * The scene is composed of the objects and the camera
 */
class Scene {
private:
  // A list of the materials available in the scene. This is stored
  // here so that materials can be "declared" in the JSON scene description
  // and re-used between primitives. For example you could declare a "white
  // light" material and have several different shapes of lights that all re-use
  // the "white light" material
  material_map materials;

  // A list of objects in the scene. Each object is a primitive
  // and an associated material from the materials vector
  // TODO: BVH
  std::vector<Object> objects;

  // Info about the camera
  Camera camera;

  /**
   * Sends a ray through the scene geometry based on the given
   * screenspace coordinate (u, v) and returns the pixel's luminance
   */
  Vec3 trace(float u, float v) const;

public:
  /**
   * Construct a scene object from the given objects and camera
   * position
   */
  Scene(std::vector<Object> &objects_a, material_map &materials_a,
        Camera &camera_a)
      : materials(materials_a), objects(objects_a), camera(camera_a){};

  /**
   * Construct a scene object from a JSON object containing the `objects` and
   * `camera` fields
   */
  Scene(const object &obj) {
    const auto material_obj = obj.at("materials").as_object();
    materials = materials_from_json(material_obj);

    const auto json_objs = obj.at("objects").as_array();
    objects.reserve(json_objs.size());
    for (const auto &o : json_objs) {
      objects.push_back(object_from_json(o.as_object(), materials));
    }

    camera = Camera(obj.at("camera").as_object());
  }

  /**
   * Cleanup!
   */
  ~Scene() {
    for (const auto &[k, v] : materials) {
      delete v;
    }

    for (const auto o : objects) {
      delete o.primitive;
    }
  }

  /**
   * Calls `trace` for each pixel in the scene for as many samples
   * as specified in the config
   */
  Image render(const Config &config) const;
};

} // namespace path_tracer

#endif // SCENE_H
