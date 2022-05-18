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
  int width, height;

public:
  Image(const int width, const int height) {
    this->width = width;
    this->height = height;
  }

  void test();
  void write(const std::string path);
};

#endif // IMAGE_H
