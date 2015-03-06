#ifndef S3D_MATH_MATH_H
#define S3D_MATH_MATH_H

#include "MathBase.h"
#include "Vector.h"
#include "Math.h"
#include "Point.h"
#include "Geometry.h"

#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace s3d
{

template<typename T>
Point4<T> operator * (const Point4<T>& pt4, const Matrix<T, 4U, 4U>& m1) {
  Point4<T> ptRes;
  ptRes.x_ = m1[0][0] * pt4.x_ + m1[1][0] * pt4.y_ + m1[2][0] * pt4.z_ + m1[3][0] * pt4.w_;
  ptRes.y_ = m1[0][1] * pt4.x_ + m1[1][1] * pt4.y_ + m1[2][1] * pt4.z_ + m1[3][1] * pt4.w_;
  ptRes.z_ = m1[0][2] * pt4.x_ + m1[1][2] * pt4.y_ + m1[2][2] * pt4.z_ + m1[3][2] * pt4.w_;
  ptRes.w_ = m1[0][3] * pt4.x_ + m1[1][3] * pt4.y_ + m1[2][3] * pt4.z_ + m1[3][3] * pt4.w_;
  assert(ptRes.w_ != 0.f);

  if (ptRes.w_ != 1.f) {
    ptRes.x_ /= ptRes.w_;
    ptRes.y_ /= ptRes.w_;
    ptRes.z_ /= ptRes.w_;
    ptRes.w_ = 1.f;
  }

  return ptRes;
}

template<typename T>
Point4<T>& operator *= (Point4<T>& pt4, const Matrix<T, 4U, 4U>& m1) {
  pt4 = pt4 * m1;
  return pt4;
}

}// s3d

#endif// S3D_MATH_MATH_H
