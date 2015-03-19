#include "stdafx.h"
#include "Renderer.h"
#include "math/Math.h"

using namespace std;

namespace s3d
{

namespace impl
{

}

Renderer::Renderer(uint32_t* buffer, int w, int h) : buffer_(buffer, w, h) {
#ifdef WIN32_GDI_RENDERDER
  hdc_ = NULL;
#endif
}


Renderer::~Renderer() {
}

void Renderer::drawLine2D_Horizontal(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  assert(p0.y_ == p1.y_);
  const int steps = abs(p1.x_ - p0.x_);
  int startX = p1.x_ < p0.x_ ? p1.x_ : p0.x_;

  for (int i = 0; i < steps; ++i) {
    buffer_.setPixel(startX++, p0.y_, c.getABGRValue());
  }
}

void Renderer::drawLine2D_Vertical(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  assert(p0.x_ == p1.x_);
  const int steps = abs(p1.y_ - p0.y_);
  int startY = p1.y_ < p0.y_ ? p1.y_ : p0.y_;

  for (int i = 0; i < steps; ++i) {
    buffer_.setPixel(p0.x_, startY++, c.getABGRValue());
  }
}

void Renderer::drawLine2D_DDA(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  const Point2<int> d = p1 - p0;
  const int steps = abs(d.y_) > abs(d.x_) ? abs(d.y_) : abs(d.x_);
  if (steps == 0)
    return;

  const float dx = float(d.x_) / float(steps);
  const float dy = float(d.y_) / float(steps);

  double x = p0.x_;
  double y = p0.y_;
  buffer_.setPixel((int)std::round(x), (int)std::round(y), c.getABGRValue());

  for (int i = 0; i < steps; ++i) {
    x += dx;
    y += dy;

    buffer_.setPixel((int)std::round(x), (int)std::round(y), c.getABGRValue());
  }
}

void Renderer::drawLine2D_Bresenham(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  const int dx = ::abs(p0.x_ - p1.x_);
  const int dy = ::abs(p0.y_ - p1.y_);

  int x = p0.x_;
  int y = p0.y_;
  buffer_.setPixel(x, y, c.getABGRValue());

  if (dx > dy) {//slope is less than 1
    int error = 2 * dy - dx;
    const int twoDy = 2 * dy;
    const int twoDyMinusDx = 2 * (dy - dx);

    int yInc = p0.y_ < p1.y_ ? 1 : -1;
    if (p0.x_ > p1.x_) {
      x = p1.x_;
      y = p1.y_;
      yInc = -yInc;
    }

    const int steps = dx;
    for (int i = 0; i < steps; ++i) {
      x++;
      if (error < 0)
        error += twoDy;
      else {
        error += twoDyMinusDx;
        y += yInc;
      }

      buffer_.setPixel(x, y, c.getABGRValue());
    }
  } else {
    int error = 2 * dx - dy;
    const int twoDx = 2 * dx;
    const int twoDxMinusDy = 2 * (dx - dy);

    int xInc = p0.x_ < p1.x_ ? 1 : -1;
    if (p0.y_ > p1.y_) {
      x = p1.x_;
      y = p1.y_;
      xInc = -xInc;
    }

    const int steps = dy;
    for (int i = 0; i < steps; ++i) {
      y++;

      if (error < 0)
        error += twoDx;
      else {
        error += twoDxMinusDy;
        x += xInc;
      }

      buffer_.setPixel(x, y, c.getABGRValue());
    }
  }
}

void Renderer::drawPixel2D(const Point2<int>& p0, const Color& c) {
  buffer_.setPixel(p0.x_, p0.y_, c.getABGRValue());
}

void Renderer::drawLine2D(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  Point2<int> cp0 = p0;
  Point2<int> cp1 = p1;
  if (!clipLine(cp0, cp1, RectI(0, 0, buffer_.getWidth() -1, buffer_.getHeight() -1)))
    return;
  
  if (cp1.x_ == cp0.x_) {
    drawLine2D_Vertical(cp0, cp1, c);
    return;
  }

  if (cp1.y_ == cp0.y_) {
    drawLine2D_Horizontal(cp0, cp1, c);
    return;
  }

  drawLine2D_Bresenham(cp0, cp1, c);
}

void Renderer::drawTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c) {
  drawLine2D(p0, p1, c);
  drawLine2D(p1, p2, c);
  drawLine2D(p2, p0, c);
}

void Renderer::fillFlatTopTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c) {
  auto dp0 = p0, dp1 = p1, dp2 = p2;

  if (dp0.y_ != dp1.y_) {
    if (dp0.y_ == dp2.y_)
      swap(dp1, dp2);
    else
      swap(dp0, dp2);
  } else {
    if (dp0.y_ == dp2.y_)
      return;
  }

  assert(dp0.y_ == dp1.y_);
  assert(dp2.y_ - dp0.y_ != 0.);
  assert(dp2.y_ - dp1.y_ != 0.);

  const double m0 = double(dp2.x_ - dp0.x_) / double(dp2.y_ - dp0.y_);
  const double m1 = double(dp2.x_ - dp1.x_) / double(dp2.y_ - dp1.y_);
  const auto steps = dp2.y_ - dp0.y_;
  int y = dp0.y_ + 1;
  double x0 = dp0.x_ + m0;
  double x1 = dp1.x_ + m1;

  drawLine2D_Horizontal(dp0, dp1, c);
  for (int i = 0; i < steps; ++i) {
    drawLine2D_Horizontal({static_cast<int>(x0 + 0.5), y}, {static_cast<int>(x1 + 0.5), y}, c);
    ++y;
    x0 += m0;
    x1 += m1;
  }
}

void Renderer::fillFlatBottomTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c) {
  auto dp0 = p0, dp1 = p1, dp2 = p2;

  if (dp0.y_ != dp1.y_) {
    if (dp0.y_ == dp2.y_)
      swap(dp1, dp2);
    else
      swap(dp0, dp2);
  } else {
    if (dp0.y_ == dp2.y_)
      return;
  }

  assert(dp0.y_ == dp1.y_);
  assert(dp2.y_ - dp0.y_ != 0.);
  assert(dp2.y_ - dp1.y_ != 0.);

  const double m0 = -double(dp2.x_ - dp0.x_) / double(dp2.y_ - dp0.y_);
  const double m1 = -double(dp2.x_ - dp1.x_) / double(dp2.y_ - dp1.y_);
  const auto steps = dp0.y_ - dp2.y_;
  int y = dp0.y_ - 1;
  double x0 = dp0.x_ + m0;
  double x1 = dp1.x_ + m1;

  drawLine2D_Horizontal(dp0, dp1, c);
  for (int i = 0; i < steps; ++i) {
    drawLine2D_Horizontal({static_cast<int>(x0 + 0.5), y}, {static_cast<int>(x1 + 0.5), y}, c);
    --y;
    x0 += m0;
    x1 += m1;
  }
}

void Renderer::fillTriangle2D(const Point2<int>& p0, const Point2<int>& p1, const Point2<int>& p2, const Color& c) {
  if ((p0.x_ == p1.x_ && p1.x_ == p2.x_) || (p0.y_ == p1.y_ && p1.y_ == p2.y_)) // triangle is a line
    return;

  auto dp0 = p0, dp1 = p1, dp2 = p2;
  if (dp0.y_ > dp1.y_)
    swap(dp0, dp1);

  if (dp1.y_ > dp2.y_)
    swap(dp1, dp2);

  if (dp0.y_ > dp1.y_)
    swap(dp0, dp1);

  assert(dp0.y_ <= dp1.y_ &&  dp1.y_ <= dp2.y_);

  if (dp0.y_ == dp1.y_) {
    return fillFlatTopTriangle2D(dp0, dp1, dp2, c);
  } else if (dp1.y_ == dp2.y_) {
    return fillFlatBottomTriangle2D(dp1, dp2, dp0, c);
  } else {
    assert(dp2.y_ - dp0.y_ != 0.);
    const double m = double(dp2.x_ - dp0.x_) / double(dp2.y_ - dp0.y_);
    const int ix = static_cast<int>(dp0.x_ + (dp1.y_ - dp0.y_)*m + 0.5);

    fillFlatBottomTriangle2D(dp0, dp1, {ix, dp1.y_}, c);
    fillFlatTopTriangle2D(dp1, {ix, dp1.y_}, dp2, c);
  }
}

namespace
{
Point2<int> intersectPoint(const unsigned pCode, const Point2<int>& p, const double dx, const double dy, int clipMinX, int clipMinY, int clipMaxX, int clipMaxY) {
  static const int clipCodeC = 0x0; //center

  static const int clipCodeW = 0x1; //west
  static const int clipCodeE = 0x2; //east
  static const int clipCodeS = 0x4; //south
  static const int clipCodeN = 0x8; //north

  static const int clipCodeNW = 0x9; //nw
  static const int clipCodeSW = 0x5; //sw
  static const int clipCodeNE = 0xA; //ne
  static const int clipCodeSE = 0x6; //se
   
  int x = p.x_, y = p.y_;
  //line (y1-y0) = mx(x1-x0)

  //for hor
  //x' = p0.x_ + (clipRect.getY() - p0.y_) * (dx / dy);
  //y' = clipRect.getY();

  //for ver
  //x' = clipRect.getX();
  //y' = p0.y_ + (clipRect.getX() - p0.x_) * (dy / dx);

  switch (pCode) {
    case clipCodeC:
    break;
    case clipCodeW:
    {
      assert(dx != 0.);
      x = clipMinX;
      y = static_cast<int>(p.y_ + (clipMinX - p.x_) * (dy / dx) + 0.5);
    }
    break;
    case clipCodeE:
    {
      assert(dx != 0.);
      x = clipMaxX;
      y = static_cast<int>(p.y_ + (clipMaxX - p.x_) * (dy / dx) + 0.5);
    }
    break;
    case clipCodeN:
    {
      assert(dy != 0.);
      x = static_cast<int>(p.x_ + (clipMinY - p.y_)  * (dx / dy) + 0.5);
      y = clipMinY;
    }
    break;
    case clipCodeS:
    {
      assert(dy != 0.);
      x = static_cast<int>(p.x_ + (clipMaxY - p.y_) * (dx / dy)+ 0.5);;
      y = clipMaxY;
    }
    break;
    case clipCodeNW:
    {
      assert(dy != 0.);
      x = static_cast<int>(p.x_ + (clipMinY - p.y_)  * (dx / dy) + 0.5);
      y = clipMinY;
      if (x < clipMinX || x > clipMaxX) {
        assert(dx != 0.);
        x = clipMinX;
        y = static_cast<int>(p.y_ + (clipMinX - p.x_) * (dy / dx) + 0.5);
      }
    }
    break;
    case clipCodeSW:
    {
      assert(dy != 0.);
      x = static_cast<int>(p.x_ + (clipMaxY - p.y_) * (dx / dy) + 0.5);;
      y = clipMaxY;
      if (x < clipMinX || x > clipMaxX) {
        assert(dx != 0.);
        x = clipMinX;
        y = static_cast<int>(p.y_ + (clipMinX - p.x_) * (dy / dx) +0.5);
      }     
    }
    break;
    case clipCodeNE:
    {
      assert(dy != 0.);
      x = static_cast<int>(p.x_ + (clipMinY - p.y_)  * (dx / dy) + 0.5);
      y = clipMinY;
      if (x < clipMinX || x > clipMaxX) {
        assert(dx != 0.);
        x = clipMaxX;
        y = static_cast<int>(p.y_ + (clipMaxX - p.x_) * (dy / dx) +0.5);
      } 
    }
    break;
    case clipCodeSE:
    {
      assert(dy != 0.);
      x = static_cast<int>(p.x_ + (clipMaxY - p.y_) * (dx / dy)+ 0.5);;
      y = clipMaxY;
      if (x < clipMinX || x > clipMaxX) {
        assert(dx != 0.);
        x = clipMaxX;
        y = static_cast<int>(p.y_ + (clipMaxX - p.x_) * (dy / dx) +0.5);
      } 
    }
    break;
  } 

  return Point2<int>(x, y);
}

}
bool Renderer::clipLine(Point2<int>& p0, Point2<int>& p1, const RectI& clipRect) {
  static const int clipCodeC = 0x0; //center

  static const int clipCodeW = 0x1; //west
  static const int clipCodeE = 0x2; //east
  static const int clipCodeS = 0x4; //south
  static const int clipCodeN = 0x8; //north

  static const int clipCodeNW = 0x9; //nw
  static const int clipCodeSW = 0x5; //sw
  static const int clipCodeNE = 0xA; //ne
  static const int clipCodeSE = 0x6; //se

  unsigned int p0Code = 0;
  unsigned int p1Code = 0;

  if (p0.x_ < clipRect.getLeft())
    p0Code |= clipCodeW;
  else if (p0.x_ > clipRect.getRight())
    p0Code |= clipCodeE;

  if (p0.y_ < clipRect.getTop())
    p0Code |= clipCodeN;
  else if (p0.y_ > clipRect.getBottom())
    p0Code |= clipCodeS;

  if (p1.x_ < clipRect.getLeft())
    p1Code |= clipCodeW;
  else if (p1.x_ > clipRect.getRight())
    p1Code |= clipCodeE;

  if (p1.y_ < clipRect.getTop())
    p1Code |= clipCodeN;
  else if (p1.y_ > clipRect.getBottom())
    p1Code |= clipCodeS;

  if (p0Code == 0 && p1Code == 0) {
    return true;
  }

  if (p0Code & p1Code) {
    return false;
  }

  //line (y1-y0) = mx(x1-x0)
  auto dx = p1.x_ - p0.x_;
  auto dy = p1.y_ - p0.y_;
  auto ipt0 = intersectPoint(p0Code, p0, dx, dy, clipRect.getX(), clipRect.getY(), clipRect.getRight(), clipRect.getBottom());
  auto ipt1 = intersectPoint(p1Code, p1, dx, dy, clipRect.getX(), clipRect.getY(), clipRect.getRight(), clipRect.getBottom());

  if (ipt0.x_ < clipRect.getX() || ipt0.x_ > clipRect.getRight() ||
      ipt1.x_ < clipRect.getX() || ipt1.x_ > clipRect.getRight() ||
      ipt0.y_ < clipRect.getY() || ipt0.y_ > clipRect.getBottom() ||
      ipt1.y_ < clipRect.getY() || ipt1.y_ > clipRect.getBottom()
      ) {

      return false;
  }

  p0 = ipt0;
  p1 = ipt1;

  return true;
}

}// namespace s3d