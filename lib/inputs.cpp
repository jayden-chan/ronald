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

#include "inputs.hpp"
#include <iostream>
#include <thread>

namespace ronald {

namespace po = boost::program_options;

void Config::print() const {
  std::cerr << "Using config: \n";

  std::cerr << "\twidth: " << width << '\n';
  std::cerr << "\theight: " << height << '\n';
  std::cerr << "\toutput: " << out << '\n';
  std::cerr << "\tinput: " << in << '\n';
  std::cerr << "\tsamples: " << samples << '\n';
  std::cerr << "\tthreads: " << threads << std::endl;
}

Config::Config(const po::variables_map &vm) {
  auto vm_width = vm["width"].as<int>();
  auto vm_height = vm["height"].as<int>();
  auto vm_out = vm["out"].as<std::string>();
  auto vm_in = vm["input-file"].as<std::string>();
  auto vm_samples = vm["samples"].as<int>();
  auto vm_threads = vm["threads"].as<int>();

  if (vm_width <= 0) {
    throw "Width must be greater than zero";
  } else if (vm_width > 20000) {
    throw "Image width is too large";
  }

  if (vm_height <= 0) {
    throw "Height must be greater than zero";
  } else if (vm_height > 20000) {
    throw "Image height is too large";
  }

  if (vm_samples <= 0) {
    throw "Number of samples must be greater than zero";
  } else if (vm_samples > 32000) {
    throw "Number of samples is too large";
  }

  const auto hw_concurr = static_cast<int>(std::thread::hardware_concurrency());
  if (vm_threads < 0) {
    throw "Number of samples must be greater than zero";
  } else if (vm_threads == 0) {
    if (hw_concurr == 0) {
      throw "Hardware concurrency value is not available on this machine";
    }
    vm_threads = hw_concurr;
  } else if (vm_threads > hw_concurr) {
    throw "Using more threads than hardware_concurrency value is not supported";
  }

  width = static_cast<size_t>(vm_width);
  height = static_cast<size_t>(vm_height);
  out = vm_out;
  in = vm_in;
  samples = static_cast<size_t>(vm_samples);
  threads = static_cast<size_t>(vm_threads);
}

} // namespace ronald
