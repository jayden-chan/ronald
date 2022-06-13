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
#include "scene.hpp"

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;
namespace pt = path_tracer;

int main(int argc, char **argv) {
  po::options_description desc("Allowed options");

  /* clang-format off */
  desc.add_options()
    ("help", "produce help message")
    ("width", po::value<size_t>()->required(), "width of the output image in pixels")
    ("height", po::value<size_t>()->required(), "height of the output image in pixels")
    ("out", po::value<std::string>()->default_value(std::string("./image.ppm")),
      "path to the output file")
    ("input-file", po::value<std::string>()->required(),
      "path to the input scene description JSON file")
    ("samples", po::value<size_t>()->required(), "number of samples per pixel")
    ("threads", po::value<size_t>()->default_value(1),
      "number of threads to spawn when running in multithreaded mode");
  /* clang-format on */

  po::positional_options_description p;
  p.add("input-file", -1);

  constexpr auto desc_written = "Usage: path_tracer [options] <scene.json>";
  po::variables_map vm;

  try {
    po::store(
        po::command_line_parser(argc, argv).options(desc).positional(p).run(),
        vm);
    po::notify(vm);
  } catch (po::error &e) {
    if (vm.count("help")) {
      std::cout << desc_written << "\n\n";
      std::cout << desc << '\n';
      return 0;
    } else {
      std::cout << desc_written << "\n\n";
      std::cout << desc << '\n';
      std::cout << "Error occurred while parsing command line arguments:\n";
      std::cout << e.what() << '\n';
      return 1;
    }
  }

  pt::Config config;
  try {
    config = pt::Config(vm);
  } catch (const char *err) {
    std::cerr << "Input validation error: " << err << '\n';
    return 1;
  }

  config.print();

  // TODO: handle all the error cases here
  // * file doesn't exist
  // * file isn't valid json
  // * anything else?
  std::ifstream input(config.in);
  std::stringstream sstr;
  while (input >> sstr.rdbuf())
    ;

  value jv = parse(sstr.str());

  pt::Scene scene(jv.as_object());

  pt::Image im = scene.render(config);
  im.write(config.out);

  // TODO: allocated primitives and materials are still being leaked here
}
