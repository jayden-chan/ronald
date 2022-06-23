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

#include <optional>

#include "math.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include "util.hpp"
#include "vec3.hpp"

namespace ronald {

const std::shared_ptr<Primitive> Primitive::from_json(const object &obj) {
  const auto type = get<std::string>(obj, "type", "primitive");

  if (type == "triangle") {
    return std::make_shared<Triangle>(obj);
  }

  if (type == "sphere") {
    return std::make_shared<Sphere>(obj);
  }

  throw std::runtime_error("Primitive must be either `triangle` or `sphere`");
}

} // namespace ronald
