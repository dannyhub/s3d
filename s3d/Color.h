#pragma once
#include <cstdint>
#include <cassert>
#include <algorithm>

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
    return color_.blue_;
  }

  uint8_t getRed() const{
    return color_.red_;
  }

  uint8_t getGreen() const{
    return color_.green_;
  }

  uint8_t getAlpha() const{
    return color_.alpha_;
  }

  void setBlue(uint8_t v) {
    color_.blue_ = v;
  }

  void setRed(uint8_t v) {
    color_.red_ = v;
  }

  void setGreen(uint8_t v) {
    color_.green_ = v;
  }

  void setAlpha(uint8_t v) {
    color_.alpha_ = v;
  }

  uint8_t getValue(unsigned int index) {
    assert(index < 4);
    return argbArray_[index];
  }

  void setValue(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0) {
    argb_ = b | ((uint16_t)g << 8) | ((uint32_t)r << 16) | ((uint32_t)a << 24);;
  }

  template<typename T>
  void setValue(T r, T g, T b, T a = 0) {
    setValue(static_cast<uint8_t>(std::min(T(255), r)), static_cast<uint8_t>(std::min(T(255), g)), static_cast<uint8_t>(std::min(T(255), b)));
  }

  void setValue(unsigned int index, uint8_t v) {
    assert(index < 4);
    argbArray_[index] = v;
  }

  template<typename T>
  Color operator* (T v) {
    Color ct;
    ct.color_.blue_ = static_cast<decltype(color_.blue_)>(std::max(color_.blue_ * v + 0.5, 255.0));
    ct.color_.green_ = static_cast<decltype(color_.green_)>(std::max(color_.green_ * v + 0.5, 255.));
    ct.color_.red_ = static_cast<decltype(color_.red_)>(std::max(color_.red_ * v + 0.5, 255.));
    return ct;
  }

  template<typename T>
  Color& operator*=(T v) {
    color_.blue_ = static_cast<decltype(color_.blue_)>(std::max(color_.blue_ * v + 0.5, 255.));
    color_.green_ = static_cast<decltype(color_.green_)>(std::max(color_.green_ * v + 0.5, 255.));
    color_.red_ = static_cast<decltype(color_.red_)>(std::max(color_.red_ * v + 0.5, 255.));
    return *this;
  }

  bool operator== (const Color& c) {
    return c.argb_ == argb_;
  }

  bool operator!= (const Color& c) {
    return c.argb_ != argb_;
  }


  //Color operator+ (const Color& cs) {
  //  Color cd;
  //  cd.color_.blue_ = static_cast<decltype(color_.blue_)>(std::max(cs.color_.blue_ + color_.blue_, 255));
  //  cd.color_.green_ = static_cast<decltype(color_.green_)>(std::max(cs.color_.green_ + color_.green_, 255));
  //  cd.color_.red_ = static_cast<decltype(color_.red_)>(std::max(cs.color_.red_ + color_.red_, 255));
  //  return cd;
  //}

  //Color& operator+=(const Color& cs) {
  //  color_.blue_ = static_cast<decltype(color_.blue_)>(std::max(cs.color_.blue_ + color_.blue_, 255));
  //  color_.green_ = static_cast<decltype(color_.green_)>(std::max(cs.color_.green_ + color_.green_, 255));
  //  color_.red_ = static_cast<decltype(color_.red_)>(std::max(cs.color_.red_ + color_.red_, 255));
  //  return *this;
  //}

private:
  union  {
    uint32_t argb_;

    struct {
      uint8_t blue_;
      uint8_t green_;
      uint8_t red_;
      uint8_t alpha_;
    }color_;

    uint8_t argbArray_[4];
  };
};



}// s3d

