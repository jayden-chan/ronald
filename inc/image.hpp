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
  void test(unsigned int width, unsigned int height);
  void write(std::string path, unsigned int width, unsigned int height);
};

#endif // IMAGE_H
