#ifndef S3D_MATHBASE_MATH_H
#define S3D_MATHBASE_MATH_H


#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace s3d
{
  const double kPI = 3.1415926535897932384626433832795;
  const double kPI_MUL_2 = kPI * 2;
  const double kPI_DIV_2 = kPI / 2.;
  
  template<typename T>
  T degreeToRadius(T) ;

  template<typename T>
  T radiusToDegree(T);

  template<>
  inline double degreeToRadius<double>(double d) {
    return  d * kPI / 180.;
  }

  template<>
  inline float degreeToRadius<float>(float d) {
    return  static_cast<float>(d * kPI / 180.);
  }

  template<>
  inline int degreeToRadius<int>(int d) {
    return  static_cast<int>(d * kPI / 180. + 0.5);
  }

  template<>
  inline double radiusToDegree<double>(double r) {
    return  r * 180. / kPI;
  }

  template<>
  inline float radiusToDegree<float>(float r) {
    return  static_cast<float>(r * 180. / kPI);
  }

  template<>
  inline int radiusToDegree<int>(int r) {
    return  static_cast<int>(r * 180. / kPI + 0.5);
  }

  inline bool equalZero(int a) {
    return a == 0;
  }

  inline bool equalZero(unsigned int a) {
    return a == 0;
  }
  
  inline bool equalZero(long a) {
    return a == 0;
  }

  inline bool equalZero(unsigned long a) {
    return a == 0;
  }

  inline bool equalZero(long long a) {
    return a == 0;
  }

  inline bool equalZero(unsigned long long a) {
    return a == 0;
  }

  inline bool equalZero(float a) {
    return ::fabs(a) < 1E-5;
  }

  inline bool equalZero(double a) {
    return ::fabs(a) < 1E-13;
  }

}// s3d

#endif// S3D_MATHBASE_MATH_H
