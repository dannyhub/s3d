#pragma once
#include "math/Math.h"
#include "Color.h"

namespace s3d
{

class Vertex {
public:
  typedef Point3F PointType;

  Vertex() {}
  ~Vertex() {}

  explicit Vertex(const PointType& pt) : point_(pt) {}
  Vertex(const Point3F& pt, const Color& c) : point_(pt), color_(c){}

  PointType getPoint() const {
    return point_;
  }

  Vector4FD getNormal_() const {
    return normal_;
  }

  Color getColor() const {
    return color_;
  }

  operator PointType() {
    return point_;
  }

  template<typename MATRIX4x4>
  void transform(const MATRIX4x4& mat) {
    //FIXME use the Point3F * MATRIX4x4
    auto pt = Point4FD(point_) * mat;
    point_ = Point3F(static_cast<float>(pt.x_), static_cast<float>(pt.y_), static_cast<float>(pt.z_));
  }

private:
  PointType point_;
  Vector4FD normal_;
  Color color_;
   
};

} //s3d