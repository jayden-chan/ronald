#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <iostream>

namespace po = boost::program_options;

class Config {
public:
  int width;
  int height;
  std::string out;
  int samples;
  int threads;

  Config() = default;
  Config(const po::variables_map vm);
  Config(const int width, const int height, const std::string out,
         const int samples, const int threads) {
    this->width = width;
    this->height = height;
    this->out = out;
    this->samples = samples;
    this->threads = threads;
  }

  void print();
};
