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
  int width;
  int height;
  std::string out;
  int samples;
  int threads;

  Config() = default;
  Config(const po::variables_map &vm);
  Config(const int width_a, const int height_a, const std::string &out_a,
         const int samples_a, const int threads_a)
      : width(width_a), height(height_a), out(out_a), samples(samples_a),
        threads(threads_a) {}

  void print() const;
};

#endif // INPUTS_H
