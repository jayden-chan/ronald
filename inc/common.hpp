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

#ifndef UTILS_H
#define UTILS_H

#include "material.hpp"
#include <stdlib.h>
#include <string>

#include <boost/json.hpp>
using namespace boost::json;

namespace ronald {

using material_map = std::unordered_map<std::string, std::shared_ptr<Material>>;

/**
 * An object is a primitive with an associated material
 */
struct Object {
  std::shared_ptr<Primitive> primitive;
  std::shared_ptr<Material> material;
};

/**
 * Represents an occurrence of a light ray hitting a primitive geometric object
 */
struct Hit {
  Intersection hit;
  std::shared_ptr<Material> material;
};

/**
 * Utility function for retrieving items from a JSON object with nicer
 * error messages for the user
 */
template <typename T>
T get(const object &obj, const std::string &key,
      const std::string &parent = "<root>") {
  try {
    return value_to<T>(obj.at(key));
  } catch (std::exception &e) {
    const auto err_str = "Failed to access key \"" + key + "\"" +
                         " from parent object \"" + parent + "\": " + e.what();
    throw std::runtime_error(err_str);
  }
}

inline value at(const object &obj, const std::string &key,
                const std::string &parent = "<root>") {
  try {
    return obj.at(key);
  } catch (std::exception &e) {
    const auto err_str = "Failed to access key \"" + key + "\"" +
                         " from parent object \"" + parent + "\": " + e.what();
    throw std::runtime_error(err_str);
  }
}

} // namespace ronald

#endif // UTILS_H
