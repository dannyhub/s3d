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

  T getX() const {
    return left_;
  }

  T getY() const {
    return top_;
  }

  T getLeft() const {
    return left_;
  }

  T getTop() const {
    return top_;
  }

  T getRight() const {
    return left_ + width_;
  }

  T getBottom() const {
    return top_ + height_;
  }

  T getWidth() const {
    return width_;
  }

  T getHeight() const {
    return height_;
  }

  void setLeft(T v) {
    left_ = v;
  }

  void setTop(T v) {
    top_ = v;
  }

  void setWidth(T v) {
    width_ = v;
  }

  void setHeight(T v) {
    height_ = v;
  }

private:
  T left_, top_;
  T width_, height_;
};

typedef Rect<int> RectI;
typedef Rect<float> RectF;
typedef Rect<double> RectLF;