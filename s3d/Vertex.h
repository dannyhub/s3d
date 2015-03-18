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

  Vertex(const PointType& pt) : point_(pt) {}
  Vertex(const Point3F& pt, const Color& c) : point_(pt), color_(c){}

  template<typename PT>
  Vertex(const PT& pt) {
    point_.x_ = pt.x_;
    point_.y_ = pt.y_;
    point_.z_ = pt.z_;
  }

  PointType getPoint() const {
    return point_;
  }

  Vector4FD getNormal_() const {
    return normal_;
  }

  Color getColor() const {
    return color_;
  }

  void setPoint(const PointType& p) {
    point_ = p;
  }

private:
  PointType point_;
  Vector4FD normal_;
  Color color_;
   
};

template<typename T>
Vertex operator * (const Vertex& v, const Matrix<T, 4U, 4U>& mat) {
  Vertex tr(v);
  //FIXME use the Point3F * MATRIX4x4
  auto pt = Point4FD(v.getPoint()) * mat;
  tr.setPoint(Point3F(static_cast<float>(pt.x_), static_cast<float>(pt.y_), static_cast<float>(pt.z_)));
  return tr;
}

} //s3d