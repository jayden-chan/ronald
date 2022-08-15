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

#include "dbg.h"
#include "image.hpp"
#include "inputs.hpp"
#include "scene.hpp"

#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  po::options_description desc("Allowed options");

  /* clang-format off */
  desc.add_options()
    ("help",                                                               "produce help message")
    ("width",      po::value<int>()        ->required(),                   "width of the output image in pixels")
    ("height",     po::value<int>()        ->required(),                   "height of the output image in pixels")
    ("out",        po::value<std::string>()->default_value("./image.ppm"), "path to the output file")
    ("input-file", po::value<std::string>()->required(),                   "path to the input scene description JSON file")
    ("samples",    po::value<int>()        ->required(),                   "number of samples per pixel")
    ("threads",    po::value<int>()        ->default_value(1),             "number of threads to spawn when running in multithreaded mode");
  /* clang-format on */

  po::positional_options_description p;
  p.add("input-file", -1);

  po::variables_map vm;

  try {
    po::store(
        po::command_line_parser(argc, argv).options(desc).positional(p).run(),
        vm);
    po::notify(vm);
  } catch (po::error &e) {
    constexpr auto desc_written = "Usage: ronald [options] <scene.json>";
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

  ronald::Config config;
  try {
    config = ronald::Config(vm);
  } catch (const char *err) {
    std::cerr << "Input validation error: " << err << '\n';
    return 1;
  }

  config.print();

  std::ifstream input(config.in);
  std::stringstream sstr;
  while (input >> sstr.rdbuf())
    ;

  // The input files will use the "jsonc" extension of JSON
  // which supports comments with "//" and trailing commas.
  // This is a superset of regular JSON so regular JSON can
  // be used as well
  parse_options opt;
  opt.allow_comments = true;
  opt.allow_trailing_commas = true;

  monotonic_resource mr;
  value jv;

  try {
    jv = parse(sstr.str(), &mr, opt);
  } catch (std::exception &e) {
    std::cout << "Error: Failed to parse JSON: " << e.what() << '\n';
    return 1;
  }

  try {
    const auto aspect_r = (float)config.width / (float)config.height;
    const auto scene = ronald::Scene::from_json(jv.as_object(), aspect_r);

    ronald::Image im;
    // Technically calling render_multi_threaded with one thread is fine, but
    // the code is a lot cleaner when it's just one thread so we'll have
    // separate methods
    if (config.threads == 1) {
      im = scene.render_single_threaded(config);
    } else {
      im = scene.render_multi_threaded(config);
    }

    im.apply_tmo();
    im.write(config.out);
  } catch (std::exception &e) {
    std::cout << "Error: Rendering failed: " << e.what() << '\n';
    return 1;
  }
}
