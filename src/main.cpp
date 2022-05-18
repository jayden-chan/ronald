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
    config = validate_inputs(vm);
  } catch (const char *err) {
    std::cerr << "Input validation error: " << err << '\n';
    return 1;
  }

  config.print();

  Image im(config.width, config.height);
  im.test();
  im.write(config.out);
}
