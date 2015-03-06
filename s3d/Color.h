#pragma once
#include <cstdint>
#include <cassert>

namespace s3d
{
const int kBludeIndex = 0;
const int kGreenIndex = 1;
const int kRedIndex = 2;
const int kAlphaIndex = 3;

class Color {
public:
  Color() : argb_(0) {
  }

  Color(uint32_t argb) : argb_(argb) {
  }

  Color(uint8_t r, uint8_t g, uint8_t b , uint8_t a = 0) {
    argb_ = b | ((uint16_t)g << 8) | ((uint32_t) r<< 16) | ((uint32_t)a << 24);
  }

  uint32_t getABGRValue() const {
    return argb_;
  }
  
  uint8_t getBlue() const{
    return argbArray_[kBludeIndex];
  }

  uint8_t getRed() const{
    return argbArray_[kRedIndex];
  }

  uint8_t getGreen() const{
    return argbArray_[kGreenIndex];
  }

  uint8_t getAlpha() const{
    return argbArray_[kAlphaIndex];
  }

  uint8_t getValue(int index) const{
    assert(index >= 0 && index < 4);
    return argbArray_[index];
  }

  void setValue(int index, uint8_t v) {
    assert(index >= 0 && index < 4);
    argbArray_[index] = v;
  }

private:
  union  {
  uint32_t argb_;
  uint8_t argbArray_[4];
  };
};

}// s3d

