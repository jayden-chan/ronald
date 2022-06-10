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

#ifndef INPUTS_H
#define INPUTS_H

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

class Config {
public:
  size_t width;
  size_t height;
  std::string out;
  size_t samples;
  size_t threads;
  Scene scene;

  Config(const po::variables_map &vm);
  Config(const size_t width_a, const size_t height_a, const std::string &out_a,
         const size_t samples_a, const size_t threads_a, const Scene scene_a)
      : width(width_a), height(height_a), out(out_a), samples(samples_a),
        threads(threads_a), scene(scene_a) {}

  void print() const;
};

#endif // INPUTS_H
