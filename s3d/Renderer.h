#pragma once
#include <cstdint>
#include "math/Point.h"
#include "Color.h"

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

#ifdef WIN32_GDI_RENDERDER
  HDC hdc_;
#endif

private:
  RendererBuffer buffer_;
};

}// namespace s3d