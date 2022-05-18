#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

struct Inputs {
  int width;
  int height;
  std::string out;
  int samples;
  int threads;
};

Inputs validate_inputs(po::variables_map vm) {
  auto width = vm["width"].as<int>();
  auto height = vm["height"].as<int>();
  auto out = vm["out"].as<std::string>();
  auto samples = vm["samples"].as<int>();
  auto threads = vm["threads"].as<int>();

  if (width <= 0) {
    throw "Width must be greater than zero";
  }

  if (height <= 0) {
    throw "Height must be greater than zero";
  }

  if (samples <= 0) {
    throw "Number of samples must be greater than zero";
  }

  if (threads != 1) {
    throw "Currently only exactly one thread is supported";
  }

  return {width, height, out, samples, threads};
}
