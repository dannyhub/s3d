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

  template<typename T, typename Filler>
  void fillTriangle2DCB(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, Filler& filler);

  template<typename T, typename Filler>
  void fillFlatTopTriangle2DCB(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, Filler& filler);

  template<typename T, typename Filler>
  void fillFlatBottomTriangle2DCB(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, Filler& filler);

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

  const auto xd = p1.x_ < p0.x_ ? p1.x_ : p0.x_;
  int  startX = static_cast<int>(std::ceil(xd));
  int  endX = static_cast<int>(std::ceil(p1.x_) - 1);
  const int y = static_cast<int>(p0.y_);

  for (; startX <= endX; ++startX) {
    buffer_.setPixel(startX, y, c.getABGRValue());
  }
}

template<typename T>
void Renderer::fillFlatTopTriangle2D(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, const Color& c) {
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

  const auto height = dp2.y_ - dp0.y_;
  assert(!equalZero(height));

  const T m_left = (dp2.x_ - dp0.x_) / height; 
  const T m_right = (dp2.x_ - dp1.x_) / height; 
  
  int ys = static_cast<int>(std::ceil(dp0.y_));
  const int ye = static_cast<int>(std::ceil(dp2.y_) - 1); 

  T x0 = dp0.x_ + m_left * (ys - dp0.y_);
  T x1 = dp1.x_ + m_right * (ys - dp0.y_);

  for (; ys <= ye; ++ys) {
    auto ldpt0 = Point2<T>{x0, (T)ys};
    auto ldpt1 = Point2<T>{x1, (T)ys};

    drawLine2D_Horizontal(ldpt0, ldpt1, c);
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

template<typename T, typename Filler>
void Renderer::fillTriangle2DCB(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, Filler& filler) {
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
    fillFlatTopTriangle2DCB(dp0, dp1, dp2, filler);
  } else if (equal(dp1.y_, dp2.y_)) {
    dp1.y_ = dp2.y_;
    fillFlatBottomTriangle2DCB(dp1, dp2, dp0, filler);
  } else {
    assert(!equal(dp2.y_, dp0.y_));
    assert(!equalZero(dp2.y_ - dp0.y_));

    const auto m = (dp2.x_ - dp0.x_) / (dp2.y_ - dp0.y_);
    const auto newx = dp0.x_ + (dp1.y_ - dp0.y_) * m;

    fillFlatBottomTriangle2DCB(dp0, Point2<T>{newx, dp1.y_ }, dp1, filler);
    fillFlatTopTriangle2DCB(dp1, Point2<T>{newx, dp1.y_}, dp2, filler);
  }
}

template<typename T, typename Filler>
void Renderer::fillFlatTopTriangle2DCB(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, Filler& filler) {
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

  const auto height = dp2.y_ - dp0.y_;
  assert(!equalZero(height));

  const T m_left = (dp2.x_ - dp0.x_) / height;
  const T m_right = (dp2.x_ - dp1.x_) / height;

  int ys = static_cast<int>(std::ceil(dp0.y_));
  const int ye = static_cast<int>(std::ceil(dp2.y_) - 1);

  T x0 = dp0.x_ + m_left * (ys - dp0.y_);
  T x1 = dp1.x_ + m_right * (ys - dp0.y_);

  for (; ys <= ye; ++ys) {
    auto ldpt0 = Point2<T>{x0, (T)ys};
    auto ldpt1 = Point2<T>{x1, (T)ys};

    filler(ldpt0, ldpt1);
    x0 += m_left;
    x1 += m_right;
  }
}

template<typename T, typename Filler>
void Renderer::fillFlatBottomTriangle2DCB(const Point2<T>& p0, const Point2<T>& p1, const Point2<T>& p2, Filler& filler) {
  auto dp0 = p0, dp1 = p1, dp2 = p2;

  if (!equal(dp0.y_, dp1.y_)) {
    if (equal(dp0.y_, dp2.y_)) {
      swap(dp1, dp2);
    } else {
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
  const auto  height = dp0.y_ - dp2.y_;
  assert(!equalZero(height) && height > 0.);

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

    filler(ldpt0, ldpt1);
    x0 += m_left;
    x1 += m_right;
  }
}

template<typename PT, typename RENDERER>
class GouraudFiller {
public:
  GouraudFiller(RENDERER& renderer, const PT& pt1, const PT& pt2, const PT& pt3, const Color& c1, const Color& c2, const Color& c3)
    : pt1_(pt1), pt2_(pt2), pt3_(pt3),
    c1_(c1), c2_(c2), c3_(c3),
    renderer_(renderer) {

    dy_ = 1;
    if (pt1_.y_ > pt2_.y_) {
      std::swap(pt1_, pt2_);
      std::swap(c1_, c2_);
     }

    if (pt2_.y_ > pt3_.y_) {
      std::swap(pt2_, pt3_);
      std::swap(c2_, c3_);
    }

    if (pt1_.y_ > pt3_.y_) {
      std::swap(pt1_, pt3_);
      std::swap(c1_, c3_);
    }

    assert(pt1_.y_ <= pt2_.y_ &&  pt2_.y_ <= pt3_.y_);

    if (equal(pt1_.y_, pt2_.y_)) {
      splity_ = pt3_.y_;
    } else if (equal(pt2_.y_, pt3_.y_)) {
      splity_ = pt3_.y_;
    } else {
      splity_ = pt2_.y_;
    }

    height_ = splity_ - pt1_.y_;

    curColorLeft_ = c1_;
    curColorRight_ = c1;

    lr_ = curColorLeft_.getRed();
    lb_ = curColorLeft_.getBlue();
    lg_ = curColorLeft_.getGreen();

    rr_ = curColorRight_.getRed();
    rb_ = curColorRight_.getBlue();
    rg_ = curColorRight_.getGreen();
     
  }

  void operator() (const PT& p0, const PT& p1) {
    assert(p0.y_ == p1.y_);
    assert(p0.x_ <= p1.x_);

    const auto xd = p1.x_ < p0.x_ ? p1.x_ : p0.x_;
    int  startX = static_cast<int>(std::ceil(xd));
    int  endX = static_cast<int>(std::ceil(p1.x_) - 1);
    const int y = static_cast<int>(p0.y_);

    const auto width = p1.x_ - p0.x_;
    const auto redi = (curColorRight_.getRed() - curColorLeft_.getRed()) / width;
    const auto bluei = (curColorRight_.getBlue() - curColorLeft_.getBlue()) / width;
    const auto greeni = (curColorRight_.getGreen() - curColorLeft_.getGreen()) / width;

    double r = curColorLeft_.getRed();
    double b = curColorLeft_.getBlue();
    double g = curColorLeft_.getGreen();

    for (; startX <= endX; ++startX) {
      Color c(static_cast<int>(std::min(255.0, r)), static_cast<int>(std::min(255.0, g)), static_cast<int>(std::min(255.0, b)));
      renderer_.drawPixel2D({startX, y}, c.getABGRValue());
       r += redi;
       b += bluei;
       g += greeni;
    }

    if (static_cast<int>(std::ceil(splity_) - 1.) == dy_) {
      height_ = pt3_.y_ - splity_;

      splity_ = -1;
      c1_ = curColorLeft_;
      c2_ = curColorRight_;
    }

    if (splity_ < 0) {
      lr_ += double(c3_.getRed() - c1_.getRed()) / height_;
      lb_ += double(c3_.getBlue() - c1_.getBlue()) / height_;
      lg_ += double(c3_.getGreen() - c1_.getGreen()) / height_;

      rr_ += double(c3_.getRed() - c2_.getRed()) / height_;
      rb_ += double(c3_.getBlue() - c2_.getBlue()) / height_;
      rg_ += double(c3_.getGreen() - c2_.getGreen()) / height_;

    } else {
      lr_ += double(c2_.getRed() - c1_.getRed()) / height_;
      lb_ += double(c2_.getBlue() - c1_.getBlue()) / height_;
      lg_ += double(c2_.getGreen() - c1_.getGreen()) / height_;

      rr_ += double(c3_.getRed() - c1_.getRed()) / height_;
      rb_ += double(c3_.getBlue() - c1_.getBlue()) / height_;
      rg_ += double(c3_.getGreen() - c1_.getGreen()) / height_;
    }
    
    curColorLeft_.setValue(lr_, lg_, lb_);
    curColorRight_.setValue(rr_, rg_, rb_ );
    if (pt3_.x_ < pt2_.x_) {
      std::swap(curColorLeft_, curColorRight_);
    }

    ++dy_;
  }

private:
  RENDERER& renderer_;
  PT pt1_; PT pt2_; PT pt3_;
  Color c1_; Color c2_; Color c3_;

  Color curColorLeft_, curColorRight_;
  double splity_;
  int dy_;
  double idy_left_, idy_right;
  double lr_, lb_, lg_, rr_, rb_, rg_;
  double height_;
};


}// namespace s3d