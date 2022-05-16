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
  unsigned int width, height;

public:
  Image(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
  }

  void test();
  void write(std::string path);
};

#endif // IMAGE_H
