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

enum TriangleType {
  kTriangleType_Invalid = 0,
  kTriangleType_FlatTop,
  kTriangleType_FlatBottom,
  kTriangleType_BottomTop
};

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

    filler(*this, ldpt0, ldpt1, kTriangleType_FlatTop);
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

    filler(*this, ldpt0, ldpt1, kTriangleType_FlatBottom);
    x0 += m_left;
    x1 += m_right;
  }
}

//pt1-3 is the points belong a triangle, splitPT is a new point to split a triangle to two flat triangles, 
//return kTriangleType_BottomTop : the triangle was split, otherwise  splitPT not change. 

template<typename PT>
TriangleType splitTriangleToFlat(const PT& pt1, const PT& pt2, const PT& pt3, PT& splitPT) {
  if ((equal(pt1.x_, pt2.x_) && equal(pt2.x_, pt3.x_)) || (equal(pt1.y_, pt2.y_) && equal(pt2.y_, pt3.y_))) //line
    return kTriangleType_Invalid;

  auto dp0 = pt1, dp1 = pt2, dp2 = pt3;
  if (dp0.y_ > dp1.y_)
    std::swap(dp0, dp1);

  if (dp1.y_ > dp2.y_)
    std::swap(dp1, dp2);

  if (dp0.y_ > dp1.y_)
    std::swap(dp0, dp1);

  assert(dp0.y_ <= dp1.y_ &&  dp1.y_ <= dp2.y_);

  if (equal(dp0.y_, dp1.y_)) {
    return kTriangleType_FlatTop;
  } else if (equal(dp1.y_, dp2.y_)) {
    return kTriangleType_FlatBottom;
  } else {
    assert(!equal(dp2.y_, dp0.y_));
    assert(!equalZero(dp2.y_ - dp0.y_));

    const auto m = (dp2.x_ - dp0.x_) / (dp2.y_ - dp0.y_);
    const auto newx = dp0.x_ + (dp1.y_ - dp0.y_) * m;
    
    splitPT.x_ = newx;
    splitPT.y_ = dp1.y_;
    return kTriangleType_BottomTop;
  }
}

template<typename PT>
class GouraudFiller {
public:
  GouraudFiller(const PT& pt1, const PT& pt2, const PT& pt3, const Color& c1, const Color& c2, const Color& c3)
    : pt1_(pt1), 
      pt2_(pt2),
      pt3_(pt3),
      c1_(c1),
      c2_(c2),
      c3_(c3) {
 
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

    PT ptSplit;
    Color curColorLeft, curColorRight;

    auto triType = splitTriangleToFlat(pt1_, pt2_, pt3_, ptSplit);
    if (kTriangleType_FlatBottom == triType) {
      if (pt3_.x_ < pt2_.x_) {
        std::swap(pt3_, pt2_);
        std::swap(c2_, c3_);
      }

      curColorLeft = c1_;
      curColorRight = c1_;
      triangleType_ = kTriangleType_FlatBottom;

    } else if (kTriangleType_FlatTop == triType) {
      if (pt2_.x_ < pt1_.x_) {
        std::swap(pt1_, pt2_);
        std::swap(c1_, c2_);
      }

      curColorLeft = c1_;
      curColorRight = c2_;
      triangleType_ = kTriangleType_FlatTop;

    } else if (kTriangleType_BottomTop == triType) {
      if (pt3_.x_ < pt2_.x_) {
        std::swap(pt3_, pt2_);
        std::swap(c2_, c3_);
      }

      pt4_ = pt3_;
      pt3_ = ptSplit;
      c4_ = c3_;

      //compute c3
      const auto h = pt4_.y_ - pt1_.y_;
      const auto dist = pt3_.y_ - pt1_.y_;

      const auto r = dist * double(c4_.getRed() - c1_.getRed()) / (h);
      const auto b = dist * double(c4_.getBlue() - c1_.getBlue()) / (h);
      const auto g = dist * double(c4_.getGreen() - c1_.getGreen()) / (h);

      c3_.setValue(r, g, b);
      curColorLeft = c1_;
      curColorRight = c1_;

      triangleType_ = kTriangleType_FlatBottom;
    } else {
      triangleType_ = kTriangleType_Invalid;
    }

    height_ = pt3_.y_ - pt1_.y_;

    lr_ = curColorLeft.getRed();
    lb_ = curColorLeft.getBlue();
    lg_ = curColorLeft.getGreen();

    rr_ = curColorRight.getRed();
    rb_ = curColorRight.getBlue();
    rg_ = curColorRight.getGreen();
     
  }

  template<typename RENDERER>
  void operator() (RENDERER& renderer_, const PT& p0, const PT& p1, TriangleType triType) {
    if (kTriangleType_Invalid == triType)
      return;

    assert(triType != kTriangleType_BottomTop);
    assert(p0.y_ == p1.y_);
    assert(p0.x_ <= p1.x_);

    int  startX = static_cast<int>(std::ceil(p0.x_));
    int  endX = static_cast<int>(std::ceil(p1.x_) - 1);
    const int y = static_cast<int>(p0.y_);

    const auto width = p1.x_ - p0.x_;
    if (equalZero(width))
      return;

    const auto redi = (rr_ - lr_) / width;
    const auto bluei = (rb_ - lb_) / width;
    const auto greeni = (rg_ - lg_) / width;

    double r = lr_; 
    double b = lb_;
    double g = lg_; 
    Color c;
    c.setValue(r, g, b);

    for (; startX <= endX; ++startX) {
      renderer_.drawPixel2D({startX, y}, c.getABGRValue());
      r += redi;
      b += bluei;
      g += greeni;
      c.setValue(r, g, b);
    }

    if (triangleType_ != triType) {
      c3_ = c4_;
      c1_ = c2_;
      c2_ = c3_;
      height_ = pt4_.y_ - pt3_.y_;
      triangleType_ = triType;
    }

    if (triType == kTriangleType_FlatBottom) {
      lr_ += double(c2_.getRed() - c1_.getRed()) / height_;
      lb_ += double(c2_.getBlue() - c1_.getBlue()) / height_;
      lg_ += double(c2_.getGreen() - c1_.getGreen()) / height_;

      rr_ += double(c3_.getRed() - c1_.getRed()) / height_;
      rb_ += double(c3_.getBlue() - c1_.getBlue()) / height_;
      rg_ += double(c3_.getGreen() - c1_.getGreen()) / height_;
    } else if (triType == kTriangleType_FlatTop) {
      lr_ += double(c3_.getRed() - c1_.getRed()) / height_;
      lb_ += double(c3_.getBlue() - c1_.getBlue()) / height_;
      lg_ += double(c3_.getGreen() - c1_.getGreen()) / height_;

      rr_ += double(c3_.getRed() - c2_.getRed()) / height_;
      rb_ += double(c3_.getBlue() - c2_.getBlue()) / height_;
      rg_ += double(c3_.getGreen() - c2_.getGreen()) / height_;
    }

  }

private:
  PT pt1_; PT pt2_; PT pt3_; PT pt4_;
  Color c1_; Color c2_; Color c3_; Color c4_;

  double lr_, lb_, lg_, rr_, rb_, rg_;
  double height_;
  TriangleType triangleType_;
};


}// namespace s3d