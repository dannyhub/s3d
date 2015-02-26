#include "stdafx.h"
#include "Renderer.h"


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

  buffer_.setPixel(p0.x_, p0.y_, c.getABGRValue());
  for (int i = 0; i < steps; ++i) {
    startX++;
    buffer_.setPixel(startX, p0.y_, c.getABGRValue());
  }
}

void Renderer::drawLine2D_Vertical(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  assert(p0.x_ == p1.x_);
  const int steps = abs(p1.y_ - p0.y_);
  int startY = p1.y_ < p0.y_ ? p1.y_ : p0.y_;

  buffer_.setPixel(p0.x_, p0.y_, c.getABGRValue());
  for (int i = 0; i < steps; ++i) {
    startY++;
    buffer_.setPixel(p0.x_, startY, c.getABGRValue());
  }
}

void Renderer::drawLine2D_DDA(const Point2<int>& p0, const Point2<int>& p1, const Color& c) {
  const Point2<int> d = p1 - p0;
  const int steps = abs(d.y_) > abs(d.x_) ? abs(d.y_) : abs(d.x_);
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
  //if (cp0.x_ < 0) 
  //  cp0.x_ = 0;

  //if (cp0.y_ < 0)
  //  cp0.y_ = 0;

  //if (cp0.x_ >= buffer_.getWidth())
  //  cp0.x_ = buffer_.getWidth()-1;

  //if (cp0.y_ >= buffer_.getHeight())
  //  cp0.y_ = buffer_.getHeight() - 1;

  Point2<int> cp1 = p1;
  //if (cp1.x_ < 0)
  //  cp1.x_ = 0;

  //if (cp1.y_ < 0)
  //  cp1.y_ = 0;

  //if (cp1.x_ >= buffer_.getWidth())
  //  cp1.x_ = buffer_.getWidth() - 1;

  //if (cp1.y_ >= buffer_.getHeight())
  //  cp1.y_ = buffer_.getHeight() - 1;

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

}// namespace s3d