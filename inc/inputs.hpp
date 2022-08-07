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

#ifndef INPUTS_H
#define INPUTS_H

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace ronald {

namespace po = boost::program_options;

class Config {
public:
  size_t width = 0;
  size_t height = 0;
  std::string out;
  std::string in;
  size_t samples = 0;
  size_t threads = 0;

  Config() = default;

  /**
   * Parse the Config from the given Boost variables map
   */
  [[nodiscard]] explicit Config(const po::variables_map &vm);

  void print() const;
};

} // namespace ronald

#endif // INPUTS_H
