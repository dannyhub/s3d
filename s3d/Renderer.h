#pragma once
#include <cstdint>
#include "math/Point.h"
#include "Color.h"
#include "Rect.h"

namespace s3d
{

class RendererBuffer {
public:
  RendererBuffer(uint32_t* buffer, int w, int h) : buffer_(buffer), width_(w), height_(h){
    assert(buffer_);
  }

  inline void setPixel(int x, int y, uint32_t p) {
    assert(y * width_ + x < width_ * height_);
    buffer_[y * width_ + x] = p;
  }

  int getWidth() const {
    return width_;
  }
  int getHeight() const {
    return height_;
  }

private:
  int width_;
  int height_;
  uint32_t* buffer_;
};

class Renderer {
public:
  Renderer(uint32_t* buffer, int w, int h);
  ~Renderer();

  void drawPixel2D(const Point2<int>& p0, const Color& c);
  void drawLine2D(const Point2<int>& p0, const Point2<int>& p1, const Color& c);

  void drawLine2D_Horizontal(const Point2<int>& p0, const Point2<int>& p1, const Color& c);
  void drawLine2D_Vertical(const Point2<int>& p0, const Point2<int>& p1, const Color& c);

  void drawLine2D_DDA(const Point2<int>& p0, const Point2<int>& p1, const Color& c);
  void drawLine2D_Bresenham(const Point2<int>& p0, const Point2<int>& p1, const Color& c);

  void drawTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c);

  void fillFlatTopTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c);
  void fillFlatBottomTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c);

  void fillTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c);

  bool clipLine(Point2<int>& p0, Point2<int>& p1, const RectI& clipRect);

  template<typename T>
  void drawLine2D_Horizontal(const Point2<T>& p0, const Point2<T>& p1, const Color& c);

  template<typename T>
  void fillFlatTopTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c);

  template<typename T>
  void fillFlatBottomTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c);

  template<typename T>
  void fillTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c);

#ifdef WIN32_GDI_RENDERDER
  HDC hdc_;
#endif

private:
  RendererBuffer buffer_;
};

template<typename T>
void Renderer::drawLine2D_Horizontal(const Point2<T>& p0, const Point2<T>& p1, const Color& c) {
  assert(p0.y_ == p1.y_);
  assert(p0.x_ <= p1.x_);

  const int steps = static_cast<int>(std::abs(std::ceil(p1.x_ - p0.x_)));
  const auto xd = p1.x_ < p0.x_ ? p1.x_ : p0.x_;

  int  startX = static_cast<int>(std::ceil(xd));
  const int y = static_cast<int>(p0.y_);

  for (int i = 0; i < steps; ++i) {
    buffer_.setPixel(startX++, y, c.getABGRValue());
  }
}

template<typename T>
void Renderer::fillFlatTopTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c) {
  if ((equal(p0.x_, p1.x_) && equal(p1.x_, p2.x_)) || (equal(p0.y_, p1.y_) && equal(p1.y_, p2.y_))) // triangle is a line
    return;

  auto dp0 = p0, dp1 = p1, dp2 = p2;

  if (!equal(dp0.y_, dp1.y_)) {
    if (equal(dp0.y_, dp2.y_))
      swap(dp1, dp2);
    else
      swap(dp0, dp2);
  } else {
    if (equal(dp0.y_, dp2.y_))
      return;
  }

  if (dp1.x_ < dp0.x_) {
    swap(dp0, dp1);
  }


  assert(dp2.y_ > dp0.y_ && dp2.y_ > dp1.y_);
  assert(equal(dp0.y_, dp1.y_));
  assert(!equal(dp2.y_, dp0.y_));
  assert(!equal(dp2.y_, dp1.y_));

  const auto height = (dp2.y_ - dp0.y_);

  assert(!equalZero(height));

  const T m_left = (dp2.x_ - dp0.x_) / height; 
  const T m_right = (dp2.x_ - dp1.x_) / height; 
  
  const auto steps = std::ceil(dp2.y_ - dp0.y_) ;
  auto y = std::ceil(dp0.y_);
  T x0 = dp0.x_ + m_left * (y - dp0.y_);
  T x1 = dp1.x_ + m_right * (y - dp0.y_);

  for (int i = 0; i < steps; ++i) {
    auto ldpt0 = Point2<T>{x0, y};
    auto ldpt1 = Point2<T>{x1, y};

    if (ldpt0.x_ < ldpt1.x_)
      drawLine2D_Horizontal(ldpt0, ldpt1, c);
    ++y;
    x0 += m_left;
    x1 += m_right;
  }
}

template<typename T>
void Renderer::fillFlatBottomTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c) {
  auto dp0 = p0, dp1 = p1, dp2 = p2;

  if (!equal(dp0.y_, dp1.y_)) {
    if (equal(dp0.y_, dp2.y_)) {
      swap(dp1, dp2);
    }
    else {
      swap(dp0, dp2);
    }
  } else {
    if (equal(dp0.y_, dp2.y_)) {
      assert(false);
      return;
    }
  }

  if (dp1.x_ < dp0.x_) {
    swap(dp0, dp1);
  }
 

  assert(dp0.x_ < dp1.x_);
  assert(dp2.y_ < dp0.y_ && dp2.y_ < dp1.y_);
  assert(equal(dp0.y_, dp1.y_));
  assert(!equal(dp2.y_, dp0.y_));
  assert(!equal(dp2.y_, dp1.y_));

  const auto  height = dp0.y_ - dp2.y_;
  assert(!equalZero(height) &&  height > 0.);

  const auto m_left = (dp0.x_ - dp2.x_) / height;
  const auto m_right = (dp1.x_ - dp2.x_) / height;
  
  int ys = static_cast<int>(std::ceil(dp2.y_));
  const int ye = static_cast<int>(std::ceil(dp0.y_) - 1);

  assert(ys >= dp2.y_);
  auto x0 = dp2.x_ + (m_left * (ys - dp2.y_));
  auto x1 = dp2.x_ + (m_right * (ys - dp2.y_));

  for (; ys <= ye; ++ys) {
    auto ldpt0 = Point2<T>{x0, (T)ys};
    auto ldpt1 = Point2<T>{x1, (T)ys};

    if (ldpt0.x_ < ldpt1.x_)
      drawLine2D_Horizontal(ldpt0, ldpt1, c);

    x0 += m_left;
    x1 += m_right;
  }
}

template<typename T>
void Renderer::fillTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c) {
  if ((equal(p0.x_, p1.x_) && equal(p1.x_, p2.x_)) || (equal(p0.y_, p1.y_) && equal(p1.y_, p2.y_))) // triangle is a line
    return;

  auto dp0 = p0, dp1 = p1, dp2 = p2;
  if (dp0.y_ > dp1.y_)
    std::swap(dp0, dp1);

  if (dp1.y_ > dp2.y_)
    std::swap(dp1, dp2);

  if (dp0.y_ > dp1.y_)
    std::swap(dp0, dp1);

  assert(dp0.y_ <= dp1.y_ &&  dp1.y_ <= dp2.y_);

  if (equal(dp0.y_, dp1.y_)) {
    dp0.y_ = dp1.y_;
    fillFlatTopTriangle2D(dp0, dp1, dp2, c);
  } else if (equal(dp1.y_, dp2.y_)) {
    dp1.y_ = dp2.y_;
    fillFlatBottomTriangle2D(dp1, dp2, dp0, c);
  } else {
    assert(!equal(dp2.y_, dp0.y_));
    assert(!equalZero(dp2.y_ - dp0.y_));

    const auto m = (dp2.x_ - dp0.x_) / (dp2.y_ - dp0.y_);
    const auto newx = dp0.x_ + (dp1.y_ - dp0.y_) * m;

    fillFlatBottomTriangle2D(dp0, Point2<T>{newx, dp1.y_ }, dp1, c);
    fillFlatTopTriangle2D(dp1, Point2<T>{newx, dp1.y_}, dp2, c);
  }
}



}// namespace s3d