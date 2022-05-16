#include "image.hpp"

void Image::test(unsigned int width, unsigned int height) {
  this->buffer.reserve(width * height);
  for (unsigned int i = 0; i < height; i++) {
    for (unsigned int j = 0; j < width; j++) {
      float r = static_cast<float>(j) / static_cast<float>(width);
      float g = static_cast<float>(i) / static_cast<float>(height);
      /* float b = static_cast<float>(i) / static_cast<float>(height); */
      float b = 0;
      /* float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); */
      /* float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); */
      /* float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); */
      this->buffer.push_back({r, g, b});
    }
  }
}

void Image::write(std::string path, unsigned int width, unsigned int height) {
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
