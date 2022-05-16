#ifndef IMAGE_H
#define IMAGE_H

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

struct Pixel {
  float r;
  float g;
  float b;
};

class Image {
  std::vector<Pixel> buffer;

public:
  void test(unsigned int width, unsigned int height) {
    this->buffer.reserve(width * height);
    for (unsigned int i = 0; i < width; i++) {
      for (unsigned int j = 0; j < height; j++) {
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        this->buffer.push_back({r, g, b});
      }
    }
  }

  void write(std::string path, unsigned int width, unsigned int height) {
    std::ofstream myfile;
    myfile.open(path);
    myfile << "P6\n";
    myfile << width << " " << height << "\n";
    myfile << "255\n";

    for (const auto p : this->buffer) {
      myfile << (unsigned char)(sqrt(p.r) * 255.0);
      myfile << (unsigned char)(sqrt(p.g) * 255.0);
      myfile << (unsigned char)(sqrt(p.b) * 255.0);
    }

    myfile.close();
  }
};

#endif // IMAGE_H
