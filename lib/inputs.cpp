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

#include "inputs.hpp"
#include <iostream>

namespace po = boost::program_options;

void Config::print() const {
  std::cerr << "Using config: \n";

  std::cerr << "\twidth: " << width << '\n';
  std::cerr << "\theight: " << height << '\n';
  std::cerr << "\toutput: " << out << '\n';
  std::cerr << "\tsamples: " << samples << '\n';
  std::cerr << "\tthreads: " << threads << std::endl;
}

Config::Config(const po::variables_map &vm) {
  auto vm_width = vm["width"].as<int>();
  auto vm_height = vm["height"].as<int>();
  auto vm_out = vm["out"].as<std::string>();
  auto vm_samples = vm["samples"].as<int>();
  auto vm_threads = vm["threads"].as<int>();

  if (vm_width <= 0) {
    throw "Width must be greater than zero";
  }

  if (vm_height <= 0) {
    throw "Height must be greater than zero";
  }

  if (vm_samples <= 0) {
    throw "Number of samples must be greater than zero";
  }

  if (vm_threads != 1) {
    throw "Currently only exactly one thread is supported";
  }

  width = vm_width;
  height = vm_height;
  out = vm_out;
  samples = vm_samples;
  threads = vm_threads;
}
