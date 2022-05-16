#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <image.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char **argv) {
  // Declare the supported options.
  po::options_description desc("Allowed options");

  std::vector<int> myVec;

  /* clang-format off */
  desc.add_options()
      ("help", "produce help message")
      ("width", po::value<unsigned int>()->required(), "width of the output image in pixels")
      ("height", po::value<unsigned int>()->required(), "height of the output image in pixels")
      ("out",
          po::value<std::string>()->default_value(std::string("./image.ppm")),
          "path to the output file")
      ("samples", po::value<unsigned int>()->required(), "number of samples per pixel")
      ("threads", po::value<unsigned int>()->default_value(1),
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

  unsigned int width = vm["width"].as<unsigned int>();
  unsigned int height = vm["height"].as<unsigned int>();
  std::string out = vm["out"].as<std::string>();
  unsigned int samples = vm["samples"].as<unsigned int>();
  unsigned int threads = vm["threads"].as<unsigned int>();

  std::cout << "Using config: \n";

  std::cout << "\twidth: " << width << "\n";
  std::cout << "\theight: " << height << "\n";
  std::cout << "\tout: " << out << "\n";
  std::cout << "\tsamples: " << samples << "\n";
  std::cout << "\tthreads: " << threads << "\n";

  Image im;
  im.test(width, height);
  im.write(out, width, height);
}
