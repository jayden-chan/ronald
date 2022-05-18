#include "image.hpp"
#include "inputs.cpp"

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
    std::cout << "Error ocurred while parsing command line arguments:\n";
    std::cout << e.what() << "\n";
    return 1;
  }

  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }

  auto inputs = validate_inputs(vm);

  std::cerr << "Using config: \n";

  std::cerr << "\twidth: " << inputs.width << "\n";
  std::cerr << "\theight: " << inputs.height << "\n";
  std::cerr << "\toutput: " << inputs.out << "\n";
  std::cerr << "\tsamples: " << inputs.samples << "\n";
  std::cerr << "\tthreads: " << inputs.threads << "\n";

  Image im(inputs.width, inputs.height);
  im.test();
  im.write(inputs.out);
}
