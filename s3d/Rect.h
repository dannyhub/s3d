#pragma once

template<typename T>
class Rect {
public:
  typedef T ValueType;

  Rect() : left_(0), top_(0), width_(0), height_(0) {
  }

  Rect(T left, T top, T width, T height) : left_(left), top_(top), width_(width), height_(height) {
  }

  ~Rect() {
  }

  void setRect(T left, T top, T width, T height) {
    left_ = left;
    top_ = top;
    width_ = width;
    height_ = height;
  }

  T x() const {
    return left_;
  }

  T y() const {
    return top_;
  }

  T left() const {
    return left_;
  }

  T top() const {
    return top_;
  }

  T right() const {
    return left_ + width_;
  }

  T bottom() const {
    return top_ + height_;
  }

  T width() const {
    return width_;
  }

  T height() const {
    return height_;
  }

  void left(T v) {
    left_ = v;
  }

  void top(T v) {
    top_ = v;
  }

  void width(T v) {
    width_ = v;
  }

  void height(T v) {
    height_ = v;
  }

private:
  T left_, top_;
  T width_, height_;
};

typedef Rect<int> RectI;
typedef Rect<float> RectF;
typedef Rect<double> RectLF;