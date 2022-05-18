#include "image.hpp"

/**
 * Test image which renders a red gradient along the X axis
 * and a green gradient along the Y axis.
 */
void Image::test() {
  buffer.reserve((size_t)width * (size_t)height);
  for (auto i = 0; i < height; i++) {
    for (auto j = 0; j < width; j++) {
      float r = static_cast<float>(j) / static_cast<float>(width);
      float g = static_cast<float>(i) / static_cast<float>(height);
      float b = 0;
      buffer.push_back({r, g, b});
    }
  }
}

/**
 * Write the image buffer to the provided file
 */
void Image::write(const std::string path) {
  std::ofstream myfile;
  myfile.open(path);

  // We will use the Binary Portable PixMap (P6) image format
  // for simplicity.
  myfile << "P6\n";
  myfile << width << " " << height << "\n";
  myfile << "255\n";

  for (const auto p : buffer) {
    myfile << (unsigned char)(sqrt(p.r) * 255.99);
    myfile << (unsigned char)(sqrt(p.g) * 255.99);
    myfile << (unsigned char)(sqrt(p.b) * 255.99);
  }

  myfile.close();
}
