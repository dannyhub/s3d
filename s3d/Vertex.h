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

  PointType point() const {
    return point_;
  }

  void point(const PointType& p) {
    point_ = p;
  }

  Vector4FD normal() const {
    return normal_;
  }

  Color color() const {
    return color_;
  }

  PointType texturePoint() const {
    return texturePoint_;
  }

  void texturePoint(const PointType& p) {
    texturePoint_ = p;
  }

private:
  PointType point_;
  Vector4FD normal_;
  Color color_;
  PointType texturePoint_;
};

template<typename T>
Vertex operator * (const Vertex& v, const Matrix<T, 4U, 4U>& mat) {
  Vertex tr(v);
  //FIXME use the Point3F * MATRIX4x4
  auto pt = Point4FD(v.point()) * mat;

  typedef Vertex::PointType::value_type value_type;
  tr.point(Point3F(static_cast<value_type>(pt.x_), static_cast<value_type>(pt.y_), static_cast<value_type>(pt.z_)));
  return tr;
}

template<typename T>
Vertex& operator *= (Vertex& v, const Matrix<T, 4U, 4U>& mat) {
  //FIXME use the Point3F * MATRIX4x4
  auto pt = Point4FD(v.point()) * mat;

  typedef Vertex::PointType::value_type value_type;
  v.point(Point3F(static_cast<value_type>(pt.x_), static_cast<value_type>(pt.y_), static_cast<value_type>(pt.z_)));
  return v;
}

} //s3d