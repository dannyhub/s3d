#ifndef S3D_MATH_COORDINATE_H
#define S3D_MATH_COORDINATE_H

#include "Point.h"
#include "Vector.h"
#include "Matrix.h"

#include <initializer_list>
#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace s3d
{

template<typename T>
class PolarPoint {
public:
  T radius_; //the radius of the point
  T angle_;  //the angle in radian

  PolarPoint(T radius, T angle) : radius_(radius), angle_(angle) {
  }

  explicit PolarPoint(const Point2<T>& pt) {
    radius_ = ::sqrt(pt.x_*pt.x_ + pt.y_*pt.y_);
    angle_ = ::atan2(pt.y_ , pt.x_);
  }
  
  Point2<T> toPoint() const {
    return {::cos(angle_) * radius_, ::sin(angle_) * radius_};
  }
};

namespace impl
{
  static const float kPI = 3.141592653f;
  static const float kPI_MUL_2 = kPI * 2.0f;
}

template<typename T>
class CylindricalPoint {
public:
  T radius_; //the radius of the point
  T angle_;  //the angle in radian
  T z_; 

  CylindricalPoint(T radius, T angle, T z) : radius_(radius), angle_(angle), z_(z) {
  }

  explicit CylindricalPoint(const Point3<T>& pt) {
    radius_ = ::sqrt(pt.x_*pt.x_ + pt.y_*pt.y_);
    angle_ = ::atan2(pt.y_ , pt.x_);
    z_ = pt.z_;
  }
  
  Point3<T> toPoint() const {
    return {::cos(angle_) * radius_, ::sin(angle_) * radius_, z_};
  }
};

template<typename T>
class SphericalPoint {
public:
  T radius_; //the radius of the point
  T anglexy_;  //the angle in radian
  T anglez_;

  SphericalPoint(T radius, T angle, T z) : radius_(radius), angle_(angle), z_(z) {
  }

  explicit SphericalPoint(const Point3<T>& pt) {
    radius_ = ::sqrt(pt.x_*pt.x_ + pt.y_*pt.y_ + pt.z_*pt.z_);
    if (impl::equalZero(radius_)) {
      radius_ = anglexy_ = anglez_ = T(0);
      return;
    }

    anglexy_ = ::atan2(pt.y_, pt.x_);
    anglez_ = ::acos(pt.z / radius_);
    z_ = pt.z_;
  }

  Point3<T> toPoint() const {
    return{::cos(anglexy_) * (radius_ * ::sin(anglez_)), ::sin(anglexy_) * (radius_ * ::sin(anglez_)), radius_* ::cos(anglez_)};
  }
};

typedef PolarPoint<float> PolarPointf;
typedef CylindricalPoint<float> CylindricalPointf;


//general line: ax + by + c = 0
template<typename T>
struct LineGeneral2D {
  T a_;
  T b_;
  T c_;

  LineGeneral2D() : a_(T(0)), b_(T(0)), c_(T(0)) {
  }

  LineGeneral2D(T a, T b, T c) : a_(a), b_(b), c_(c) {
  }

  /*
  M = (p1.y - p0.y) / (p1.x - p0.x)
  y - y0 = M(x - x0)
  y - y0 = Mx - Mx0
  -Mx -1y + (Mx0 - y0) = 0
  a_ = -M
  b_ = 1
  c_ = Mx0 - y0
  */
  LineGeneral2D(const Point2<T>& p0, const Point2<T>& p1) {
    const auto M = (p1.y - p0.y) / (p1.x - p0.x)
    a_ = -M;
    b_ = T(1);
    c_ = M * p0.x - p0.y;
  }

  T solveY(T x) const {
    return -(a_ * x + c_)
  }
};

//point-slope line: mx + b = y
template<typename T>
struct LinePointSlope2D {
  T m_;
  T b_;

  LinePointSlope2D() : m_(T(0)), b_(T(0)) {
  }

  LinePointSlope2D(const Point2<T>& p0, const Point2<T>& p1) {
    m_ = (p1.y - p0.y) / (p1.x - p0.x);
    b_ = p0.y - m_ * p0.x;
  }

  LineGeneral2D<T> toLineGeneral2D() const {
    return LineGeneral2D<T>(m, T(-1), b);
  }

  T solveY(T x) const {
    return (m_ * x + b_)
  }
};

//parametric line: l = point_ + vector_ * t , t in [0,1]
template<typename T>
struct LineParametric2D {
  Point2<T> point_;
  Point2<T> point1_;
  Vector2 vector_;

  LineParametric2D(const Point2<T>& p0, const Point2<T>& p1) : point_(p0), point1_(p1), vector(point1_ - point_) {
  }

  Vector2 compute(const T t) {
    return Vector2(point_.x_ + vector_.x_*t, point_.y_ + vector_.y_*t);
  }

  LineGeneral2D<T> toLineGeneral2D() const {
    return LineGeneral2D<T>(point_, point1_);
  }

  LinePointSlope2D<T> toLinePointSlope2D() const {
    return LinePointSlope2D<T>(point_, point1_);
  }

};

//parametric line: l = point_ + vector_ * t , t in [0,1]
template<typename T>
struct LineParametric3D {
  Point3<T> point_;
  Point3<T> point1_;
  Vector3 vector_;

  LineParametric3D(const Point3<T>& p0, const Point3<T>& p1) : point_(p0), point1_(p1), vector(point1_ - point_) {
  }

  Vector3 compute(const T t) {
    return Vector2(point_.x_ + vector_.x_*t, point_.y_ + vector_.y_*t, point_.z_ + vector_.z_*t);
  }
};


template <typename T>
bool isIntersect(const LineGeneral2D<T>& la, const LineGeneral2D<T>& lb) {
  const Matrix<T, 2U, 2U> matC = {la.a_, la.b_,
                                  lb.a_, lb.b_};

  return !vector_impl::equalZero(matC.det());
}

template <typename T>
Vector2<T> intersect(const LineGeneral2D<T>& la, const LineGeneral2D<T>& lb) {
  const Matrix<T, 2U, 2U> matC = { la.a_, la.b_,
                                   lb.a_, lb.b_ };

  const Matrix<T, 1U, 2U> matY = { -la.c_, 
                                   -lb.c_ };
  const auto resMat = matrixSolve(matC, matY);
  return Vector2<T>(resMat.at(0, 0), resMat.at(0, 1));
}


//for LineParametric2D
//x1 = la.point_.x + la.vector_.x * t1
//y1 = la.point_.y + la.vector_.y * t1

//x2 = lb.point_.x + lb.vector_.x * t2
//y2 = lb.point_.y + lb.vector_.y * t2

//x1 = x2, la.point_.x + la.vector_.x * t1 = lb.point_.x + lb.vector_.x * t2
//y1 = y2, la.point_.y + la.vector_.y * t1 = lb.point_.y + lb.vector_.y * t2

//la.vector_.x * t1 - lb.vector_.x * t2 + la.point_.x - lb.point_.x = 0;
//la.vector_.y * t1 - lb.vector_.y * t2 + la.point_.y - lb.point_.y = 0;
template <typename T>
inline
bool isSignInvese(T a, T b) {
  if (a < T(0)) {
    return b >= T(0);
  } else {
    return b <= T(0);
  }
}

template <typename T>
bool isIntersect(const LineParametric2D<T>& la, const LineParametric2D<T>& lb) {
  const Matrix<T, 2U, 2U> matC = {la.vector_.x, -lb.vector_.x,
                                  la.vector_.y, -lb.vector_.y};

  const Matrix<T, 1U, 2U> matY = {lb.point_.x - la.point_.x, lb.point_.y - la.point_.y};
  try {
    T tMat = matrixSolve(matC, matY);
    const T t1 = tMat.at(0, 0);
    const T t2 = tMat.at(0, 1);

    if (t1 < T(0) || t1 > T(1) || t2 < T(0) || t2 > T(1)) {
      return false;
    }

    return true;
  } catch (MatrixInverseException&) {
    //Intersect whith a or more points
    if (vector_impl::equalZero(la.toLinePointSlope2D().b_ - lb.toLinePointSlope2D().b_)) {
      if (vector_impl::equalZero(la.point_.x_ - lb.point_.x_)) {
        return ::fabs();
      }
      else if (isSignInvese(la.point_.x_ - lb.point_.x_, la.point_.x_ - lb.point1_.x_))
        return true;
      else if (isSignInvese(lb.point_.x_ - la.point_.x_, lb.point_.x_ - la.point1_.x_))
        return true
      else 
        return false;
    }
  }

  return false;
}

template <typename T>
bool intersect(const LineParametric2D<T>& la, const LineParametric2D<T>& lb, Vector2<T>* resultVec) {
  
  const Matrix<T, 2U, 2U> matC = {la.vector_.x, -lb.vector_.x,
                                  la.vector_.y, -lb.vector_.y};

  const Matrix<T, 1U, 2U> matY = {lb.point_.x - la.point_.x, lb.point_.y - la.point_.y};
  try {
    T tMat = matrixSolve(matC, matY);
    const T t1 = tMat.at(0, 0);
    const T t2 = tMat.at(0, 1);

    if (t1 < T(0) || t1 > T(1) || t2 < T(0) || t2 > T(1)) {
      return false;
    }

    resultVec->x = la.point_.x + la.vector_.x * t1;
    resultVec->y = la.point_.y + la.vector_.y * t1;
    return true;
  } catch (MatrixInverseException&) {
  }

  return false;
}


//for LineParametric3D
//x1 = la.point_.x + la.vector_.x * t1
//y1 = la.point_.y + la.vector_.y * t1
//z1 = la.point_.z + la.vector_.z * t1

//x2 = lb.point_.x + lb.vector_.x * t2
//y2 = lb.point_.y + lb.vector_.y * t2
//z2 = lb.point_.y + lb.vector_.z * t2

//x1 = x2, la.point_.x + la.vector_.x * t1 = lb.point_.x + lb.vector_.x * t2
//y1 = y2, la.point_.y + la.vector_.y * t1 = lb.point_.y + lb.vector_.y * t2
//z1 = z2, la.point_.z + la.vector_.z * t1 = lb.point_.z + lb.vector_.z * t2

//la.vector_.x * t1 - lb.vector_.x * t2 + la.point_.x - lb.point_.x = 0;
//la.vector_.y * t1 - lb.vector_.y * t2 + la.point_.y - lb.point_.y = 0;
//la.vector_.z * t1 - lb.vector_.z * t2 + la.point_.z - lb.point_.z = 0;

template <typename T>
bool isIntersect(const LineParametric3D<T>& la, const LineParametric3D<T>& lb) {
  const Matrix<T, 2U, 2U> matC = {la.vector_.x, -lb.vector_.x,
                                  la.vector_.y, -lb.vector_.y};

  const Matrix<T, 1U, 2U> matY = {lb.point_.x - la.point_.x, lb.point_.y - la.point_.y};
  try {
    T tMat = matrixSolve(matC, matY);
    const T t1 = tMat.at(0, 0);
    const T t2 = tMat.at(0, 1);

    if (t1 < T(0) || t1 > T(1) || t2 < T(0) || t2 > T(1)) {
      return false;
    }

    const auto laz = la.point_.z + la.vector_.z * t1;
    const auto lbz = lb.point_.z + lb.vector_.z * t2;

    if (!vector_impl::equalZero(laz - lbz)) {
      return false;
    }

    return true;
  } catch (MatrixInverseException&) {
  }

  return false;
}


template <typename T>
bool intersect(const LineParametric3D<T>& la, const LineParametric3D<T>& lb, Point3<T>* resultVec) {
  const Matrix<T, 2U, 2U> matC = { la.vector_.x, -lb.vector_.x,
                                   la.vector_.y, -lb.vector_.y };

  const Matrix<T, 1U, 2U> matY = {lb.point_.x - la.point_.x, lb.point_.y - la.point_.y};
  try {
    T tMat = matrixSolve(matC, matY);
    const T t1 = tMat.at(0, 0);
    const T t2 = tMat.at(0, 1);

    if (t1 < T(0) || t1 > T(1) || t2 < T(0) || t2 > T(1)) {
          return false;
    }

    const auto laz = la.point_.z + la.vector_.z * t1;
    const auto lbz = lb.point_.z + lb.vector_.z * t2;

    if (!vector_impl::equalZero(laz - lbz)) {
      return false;
    }

    resultVec->z = laz;
    resultVec->x = la.point_.x + la.vector_.x * t1;
    resultVec->y = la.point_.y + la.vector_.y * t1;

    return true;
  } catch (MatrixInverseException&) {
  }

  return false;
}


//p = n_ * (X - point_) = 0;
//p = n_.x * (X.x - point_.x) +  n_.y * (X.y - point_.y) +  n_.z * (X.z - point_.z) = 0

//normalize
//p = (n_.x * X.x -n_.x * point_.x) + (n_.y * X.y -n_.y * point_.y)  + (n_.z * X.z -n_.z * point_.z) = 0
//p = n_.x * X.x + n_.y * X.y + n_.z * X.z + (-n_.x * point_.x + -n_.y * point_.y + -n_.z * point_.z)
//D = (-n_.x * point_.x + -n_.y * point_.y + -n_.z * point_.z)
//p = n_.x * X.x + n_.y * X.y + n_.z * X.z + D

template <typename T>
class Plane3D {
public:
  Point3<T> point_;
  Vector3<T> n_;

  Plane3D() {
  }

  Plane3D(const Point3<T>& p, const Vector3<T>& n, bool normalize = false) : point_(p), n_(n) {
    if (normalize)
      n_.normalizeSelf();
  }
};

//la.point_.z + la.vector_.z * t1 = 0
//t = -la.point_.z / la.vector_.z
template <typename T>
bool isIntersectXY(const LineParametric3D<T>& line) {
  const auto t = -la.point_.z / la.vector_.z;
  if (t > T(0) && t < T(1)) {
    return true;
  } else {
    return false;
  }
}

//la.point_.y + la.vector_.y * t1 = 0
//t = -la.point_.y / la.vector_.y
template <typename T>
bool isIntersectXZ(const LineParametric3D<T>& line) {
  const auto t = -la.point_.y / la.vector_.y;
  if (t > T(0) && t < T(1)) {
    return true;
  } else {
    return false;
  }
}

//la.point_.x + la.vector_.x * t1 = 0
//t = -la.point_.x / la.vector_.x
template <typename T>
bool isIntersectYZ(const LineParametric3D<T>& line) {
  const auto t = -la.point_.x / la.vector_.x;
  if (t > T(0) && t < T(1)) {
    return true;
  } else {
    return false;
  }
}

template <typename T>
inline bool isPointInPlane(const Plane3D<T>& plane, const Vector3<T>& X) {
  const auto s = plane.n_.x * (X.x - plane.point_.x) + n_.y * (X.y - plane.point_.y) + n_.z * (X.z - plane.point_.z);
  return vector_impl::equalZero(s);
}

template <typename T>
inline bool isPointOnPlanePositiveSide(const Plane3D<T>& plane, const Vector3<T>& X) {
  const auto s = plane.n_.x * (X.x - plane.point_.x) + n_.y * (X.y - plane.point_.y) + n_.z * (X.z - plane.point_.z);
  return s > T(0);
}

template <typename T>
inline bool isPointOnPlaneNegativeSide(const Plane3D<T>& plane, const Vector3<T>& X) {
  const auto s = plane.n_.x * (X.x - plane.point_.x) + n_.y * (X.y - plane.point_.y) + n_.z * (X.z - plane.point_.z);
  return s < T(0);
}

//D = (-n_.x * point_.x + -n_.y * point_.y + -n_.z * point_.z)
//p = n_.x * X.x + n_.y * X.y + n_.z * X.z + D
template <typename T>
bool isPlaneIntersect(const Plane3D<T>& la, const Plane3D<T>& lb) {
  return false;
}

//x = line.point_.x + line.vector_.x * t1
//y = line.point_.y + line.vector_.y * t1
//z = line.point_.z + line.vector_.z * t1

//D = (plane.-n_.x * plane.point_.x + -plane.n_.y * plane.point_.y + -plane.n_.z * plane.point_.z)
//p = plane.n_.x * X.x + plane.n_.y * X.y + plane.n_.z * X.z + D

//p = plane.n_.x * (line.point_.x + line.vector_.x * t1) + plane.n_.y * (line.point_.y + line.vector_.y * t1) + plane.n_.z * (line.point_.z + line.vector_.z * t1) + D
//p = plane.n_.x * line.point_.x + plane.n_.x * line.vector_.x * t1 +
//    plane.n_.y * line.point_.y + plane.n_.y * line.vector_.y * t1 +
//    plane.n_.z * line.point_.z + plane.n_.z * line.vector_.z * t1 + D

//t1 = -(plane.n_.x * line.point_.x + plane.n_.y * line.point_.y + plane.n_.z * line.point_.z + D) / (plane.n_.x * line.vector_.x + plane.n_.y * line.vector_.y + plane.n_.z * line.vector_.z)
template <typename T>
bool isIntersect(const Plane3D<T>& plane, const LineParametric3D<T>& line) {
  const auto planeDotLine = plane.n_.dotProduct(line.vector_);
  if (vector_impl::equalZero(planeDotLine)) {
    if (isPointInPlane(plane, line)) {

    } else {

    }
  }

  const T D = (-plane.n_.x * plane.point_.x + -plane.n_.y * plane.point_.y + -plane.n_.z * plane.point_.z);
  const auto t = -(plane.n_.x * line.point_.x + plane.n_.y * line.point_.y + plane.n_.z * line.point_.z + D) / (plane.n_.x * line.vector_.x + plane.n_.y * line.vector_.y + plane.n_.z * line.vector_.z);
  if (t > T(0) && t < T(1)) {
    return true;
  } else {
    return false;
  }
}

template <typename T>
inline bool isIntersect(const LineParametric3D<T>& line, const Plane3D<T>& plane) {
  return isIntersect(line, plane);
}


}// namespace s3d

#endif // !S3D_MATH_COORDINATE_H


