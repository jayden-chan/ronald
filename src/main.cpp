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

#include "image.hpp"
#include "inputs.hpp"

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  po::options_description desc("Allowed options");

  /* clang-format off */
  desc.add_options()
    ("help", "produce help message")
    ("width", po::value<int>()->required(), "width of the output image in pixels")
    ("height", po::value<int>()->required(), "height of the output image in pixels")
    (
      "out",
      po::value<std::string>()->default_value(std::string("./image.ppm")),
      "path to the output file"
    )
    ("samples", po::value<int>()->required(), "number of samples per pixel")
    (
      "threads", po::value<int>()->default_value(1),
      "number of threads to spawn when running in multithreaded mode"
    );
  /* clang-format on */

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (po::error &e) {
    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 0;
    } else {
      std::cout << "Error ocurred while parsing command line arguments:\n";
      std::cout << e.what() << "\n";
      return 1;
    }
  }

  Config config;
  try {
    config = Config(vm);
  } catch (const char *err) {
    std::cerr << "Input validation error: " << err << '\n';
    return 1;
  }

  config.print();

  Image im(config.width, config.height);
  im.test();
  im.write(config.out);
}
