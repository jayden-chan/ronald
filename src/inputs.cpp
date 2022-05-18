#include "inputs.hpp"
#include <iostream>

namespace po = boost::program_options;

void Config::print() {
  std::cerr << "Using config: \n";

  std::cerr << "\twidth: " << width << "\n";
  std::cerr << "\theight: " << height << "\n";
  std::cerr << "\toutput: " << out << "\n";
  std::cerr << "\tsamples: " << samples << "\n";
  std::cerr << "\tthreads: " << threads << "\n";
}

Config validate_inputs(po::variables_map vm) {
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

  return Config(width, height, out, samples, threads);
}
