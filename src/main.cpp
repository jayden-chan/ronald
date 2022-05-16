#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  // Declare the supported options.
  po::options_description desc("Allowed options");
  std::string out;
  unsigned int threads;

  /* clang-format off */
  desc.add_options()
      ("help", "produce help message")
      ("width", po::value<unsigned int>()->required(), "width of the output image in pixels")
      ("height", po::value<unsigned int>()->required(), "height of the output image in pixels")
      ("out",
        po::value<std::string>(&out)->default_value(std::string("./image.ppm")),
        "path to the output file")
      ("samples", po::value<unsigned int>()->required(), "number of samples per pixel")
      ("threads", po::value<unsigned int>(&threads)->default_value(1),
          "number of threads to spawn when running in multithreaded mode");
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

  std::cout << "Using config: \n";

  std::cout << "\twidth: " << vm["width"].as<unsigned int>() << "\n";
  std::cout << "\theight: " << vm["height"].as<unsigned int>() << "\n";
  std::cout << "\tout: " << vm["out"].as<std::string>() << "\n";
  std::cout << "\tsamples: " << vm["samples"].as<unsigned int>() << "\n";
  std::cout << "\tthreads: " << vm["threads"].as<unsigned int>() << "\n";
}
