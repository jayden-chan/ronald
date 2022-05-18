#include "inputs.hpp"
#include <iostream>

namespace po = boost::program_options;

void Config::print() {
  std::cerr << "Using config: \n";

  std::cerr << "\twidth: " << width << "\n";
  std::cerr << "\theight: " << height << "\n";
  std::cerr << "\toutput: " << out << "\n";
  std::cerr << "\tsamples: " << samples << "\n";
  std::cerr << "\tthreads: " << threads << std::endl;
}

Config::Config(const po::variables_map vm) {
  auto vm_width = vm["width"].as<int>();
  auto vm_height = vm["height"].as<int>();
  auto vm_out = vm["out"].as<std::string>();
  auto vm_samples = vm["samples"].as<int>();
  auto vm_threads = vm["threads"].as<int>();

  if (vm_width <= 0) {
    throw "Width must be greater than zero";
  }

  if (vm_height <= 0) {
    throw "Height must be greater than zero";
  }

  if (vm_samples <= 0) {
    throw "Number of samples must be greater than zero";
  }

  if (vm_threads != 1) {
    throw "Currently only exactly one thread is supported";
  }

  this->width = vm_width;
  this->height = vm_height;
  this->out = vm_out;
  this->samples = vm_samples;
  this->threads = vm_threads;
}
