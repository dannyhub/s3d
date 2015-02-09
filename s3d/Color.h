#pragma once
#include <cstdint>

namespace s3d
{

class Color {
public:
  Color() : argb_(0) {
  }

  Color(uint32_t argb) : argb_(argb) {
  }

  Color(uint8_t r, uint8_t g, uint8_t b , uint8_t a = 0) {
    argb_ = r | ((uint16_t)g << 8) | ((uint32_t)b << 16) | ((uint32_t)a << 24);
  }

  uint32_t getABGRValue() const {
    return argb_;
  }


private:
  uint32_t argb_;
};

}// s3d

