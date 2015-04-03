#pragma once
#include "Color.h"

#include <vector>
namespace s3d
{

class BMPBitmap
{
public:
  void fromFile(const std::string& filename);

  int getWidth() const {
    return width_;
  }

  int getHeight() const {
    return height_;
  }

  Color getPixel(int x, int y) {
    return buffer_[y*width_ + x];
  }

  void setPixel(int x, int y,  uint32_t c) {
    buffer_[y*width_ + x] = c;
  }

private:
  int width_ = 0;
  int height_ = 0;
  std::vector<uint32_t> buffer_;
};



}//s3d
