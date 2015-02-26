#ifndef S3D_MATH_VECTOR_H
#define S3D_MATH_VECTOR_H

#include <initializer_list>
#include <cassert>
#include <algorithm>
#include <stdexcept>

#include <cmath>
#include <cfloat>

namespace s3d
{

class VectorDivideZeroException : public std::exception {
public:
  VectorDivideZeroException(const char * const & msg) : exception(msg) {
  }
};

template<typename T>
class Vector2 {
public:
  T x_;
  T y_;

public:
  Vector2() : x_(T(0)), y_(T(0)) {
  }

  Vector2(T x, T y) : x_(x), y_(y) {
  }

  Vector2(const std::initializer_list<T>& ilist) {
    assert(ilist.size() == 2);

    auto bit = ilist.begin();
    x_ = *bit++;
    y_ = *bit;
  }

  T length() const;
  Vector2 normalize() const;
  void normalizeSelf() {
    const T len = this->length();
    if (vector_impl::equalZero(len)) {
      throw VectorDivideZeroException("Vector3 length is zero");
    }

    x_ /= len;
    y_ /= len;
  }

  T dotProduct(const Vector2& v) const ;
  T cos(const Vector2& v) const;
  T angle(const Vector2& v) const;

  Vector2 projectOnto(const Vector2& v) const;
};

template<typename T>
class Vector3 {
public:
  T x_;
  T y_;
  T z_;

public:
  Vector3() : x_(T(0)), y_(T(0)), z_(T(0)) {
  }

  Vector3(T x, T y, T z) : x_(x), y_(y), z_(z) {
  }

  Vector3(const std::initializer_list<T>& ilist) {
    assert(ilist.size() == 3);

    auto bit = ilist.begin();
    x_ = *bit++;
    y_ = *bit++;
    z_ = *bit;
  }

  T length() const;
  Vector3 normalize() const;

  void normalizeSelf() {
    const T len = this->length();
    if (vector_impl::equalZero(len)) {
      throw VectorDivideZeroException("Vector3 length is zero");
    }

    x_ /= len;
    y_ /= len;
    z_ /= len;
  }

  T dotProduct(const Vector3& v) const;
  T cos(const Vector3& v) const;
  T angle(const Vector3& v) const;

  Vector3 projectOnto(const Vector3& v) const;

  Vector3 crossProduct(const Vector3& v) const;
};

template<typename T>
class Vector4 {
public:
  T x_;
  T y_;
  T z_;
  T w_;

public:
  Vector4() : x_(T(0)), y_(T(0)), z_(T(0)), w_(T(1)) {
  }

  Vector4(T x, T y, T z) : x_(x), y_(y), z_(z), w_(T(1)) {
  }

  explicit Vector4(const Vector3<T>& v3) : x_(v3.x_), y_(v3.y_), z_(v3.z_), w_(T(1)) {
  }

  Vector4(const std::initializer_list<T>& ilist) {
    assert(ilist.size() == 3);

    auto bit = ilist.begin();
    x_ = *bit++;
    y_ = *bit++;
    z_ = *bit;
    w_ = T(1);
  }

  T length() const;
  Vector4 normalize() const;
  void normalizeSelf() {
    const T len = this->length();
    if (vector_impl::equalZero(len)) {
      throw VectorDivideZeroException("Vector4 length is zero");
    }

    x_ /= len;
    y_ /= len;
    z_ /= len;
  }

  T dotProduct(const Vector4& v) const;
  T cos(const Vector4& v) const;
  T angle(const Vector4& v) const;

  Vector4 projectOnto(const Vector4& v) const;

  Vector4 crossProduct(const Vector4& v) const;
};

namespace vector_impl
{

  inline bool equalZero(int a) {
    return a == 0;
  }

  inline bool equalZero(float a) {
    return ::fabs(a) < 1E-5;
  }

  inline bool equalZero(double a) {
    return ::fabs(a) < 1E-13;
  }

}// vector_impl

template<typename T>
bool operator == (const Vector2<T>& v1, const Vector2<T>& v2) {
  return vector_impl::equalZero(v1.x_ - v2.x_) && vector_impl::equalZero(v1.y_ - v2.y_);
}

template<typename T>
inline bool operator != (const Vector2<T>& v1, const Vector2<T>& v2) {
  return !(v1 == v2);
}

template<typename T>
Vector2<T> operator + (const Vector2<T>& v1, const Vector2<T>& v2) {
  return Vector2<T>(v1.x_ + v2.x_, v1.y_ + v2.y_);
}

template<typename T>
Vector2<T>& operator += (Vector2<T>& v1, const Vector2<T>& v2) {
  v1.x_ += v2.x_;
  v1.y_ += v2.y_;

  return v1;
}

template<typename T>
Vector2<T> operator - (const Vector2<T>& v1, const Vector2<T>& v2) {
  return Vector2<T>(v1.x_ - v2.x_, v1.y_ - v2.y_);;
}

template<typename T>
Vector2<T>& operator -= (Vector2<T>& v1, const Vector2<T>& v2) {
  v1.x_ -= v2.x_;
  v1.y_ -= v2.y_;

  return v1;
}

template<typename T, typename ValueT>
Vector2<T> operator * (const Vector2<T>& v1, ValueT value) {
  return  Vector2<T>(v1.x_ * value, v1.y_ * value);
}

template<typename T, typename ValueT>
inline Vector2<T> operator * (ValueT value, const Vector2<T>& v1) {
  return operator *(v1, value);
}

template<typename T, typename ValueT>
Vector2<T>& operator *= (Vector2<T>& v1, ValueT value) {
  v1.x_ *= value;
  v1.y_ *= value;

  return v1;
}

template<typename T>
T Vector2<T>::length() const {
  return ::sqrt(x_*x_ + y_*y_);
}

template<typename T>
Vector2<T> Vector2<T>::normalize() const {
  Vector2<T> v(*this);
  const T len = this->length();
  if (vector_impl::equalZero(len)) {
    throw VectorDivideZeroException("Vector2 length is zero");
  }

  v.x_ /= len;
  v.y_ /= len;
  return v;
}

template<typename T>
inline T Vector2<T>::dotProduct(const Vector2<T>& v2) const {
  return x_ * v2.x_ + y_ * v2.y_;
}

template<typename T>
inline T Vector2<T>::cos(const Vector2<T>& v2) const {
  const auto v1len = this->length();
  const auto v2len = v2.length();
  return this->dotProduct(v2) / (v1len * v2len);
}

template<typename T>
inline T Vector2<T>::angle(const Vector2<T>& v2) const {
  return ::acos(this->cos(v2));
}

template<typename T>
inline Vector2<T> Vector2<T>::projectOnto(const Vector2<T>& v2) const {
  //this->length() * this->cos(v2) * v2.normalize();
  auto norV2 = v2.normalize();
  return norV2 * this->dotProduct(norV2);
}

///////////////////////////////////////////vector3/////////////////////////////////////

template<typename T>
bool operator == (const Vector3<T>& v1, const Vector3<T>& v2) {
  return vector_impl::equalZero(v1.x_ - v2.x_) && vector_impl::equalZero(v1.y_ - v2.y_) && vector_impl::equalZero(v1.z_ - v2.z_);
}

template<typename T>
inline bool operator != (const Vector3<T>& v1, const Vector3<T>& v2) {
  return !(v1 == v2);
}

template<typename T>
Vector3<T> operator + (const Vector3<T>& v1, const Vector3<T>& v2) {
  return Vector3<T>(v1.x_ + v2.x_, v1.y_ + v2.y_, v1.z_ + v2.z_);
}

template<typename T>
Vector3<T>& operator += (Vector3<T>& v1, const Vector3<T>& v2) {
  v1.x_ += v2.x_;
  v1.y_ += v2.y_;
  v1.z_ += v2.z_;
  return v1;
}

template<typename T>
Vector3<T> operator - (const Vector3<T>& v1, const Vector3<T>& v2) {
  return Vector3<T>(v1.x_ - v2.x_, v1.y_ - v2.y_, v1.z_ - v2.z_);
}

template<typename T>
Vector3<T>& operator -= (Vector3<T>& v1, const Vector3<T>& v2) {
  v1.x_ -= v2.x_;
  v1.y_ -= v2.y_;
  v1.z_ -= v2.z_;
  return v1;
}

template<typename T, typename ValueT>
Vector3<T> operator * (const Vector3<T>& v1, ValueT value) {
  return  Vector3<T>(v1.x_ * value, v1.y_ * value, v1.z_ * value);
}

template<typename T, typename ValueT>
inline Vector3<T> operator * (ValueT value, const Vector3<T>& v1) {
  return operator *(v1, value);
}

template<typename T, typename ValueT>
Vector3<T>& operator *= (Vector3<T>& v1, ValueT value) {
  v1.x_ *= value;
  v1.y_ *= value;
  v1.z_ *= value;
  return v1;
}

template<typename T>
T Vector3<T>::length() const {
  return ::sqrt(x_*x_ + y_*y_ + z_*z_);
}

template<typename T>
Vector3<T> Vector3<T>::normalize() const {
  Vector3<T> v(*this);
  const T len = this->length();
  if (vector_impl::equalZero(len)) {
    throw VectorDivideZeroException("Vector3 length is zero");
  }

  v.x_ /= len;
  v.y_ /= len;
  v.z_ /= len;
  return v;
}

template<typename T>
inline T Vector3<T>::dotProduct(const Vector3<T>& v2) const {
  return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
}

template<typename T>
inline T Vector3<T>::cos(const Vector3<T>& v2) const {
  const auto v1len = this->length();
  const auto v2len = v2.length();
  return this->dotProduct(v2) / (v1len * v2len);
}

template<typename T>
inline T Vector3<T>::angle(const Vector3<T>& v2) const {
  return ::acos(this->cos(v2));
}

template<typename T>
inline Vector3<T> Vector3<T>::projectOnto(const Vector3<T>& v2) const {
  //this->length() * this->cos(v2) * v2.normalize();
  auto norV2 = v2.normalize();
  return norV2 * this->dotProduct(norV2);
}

template<typename T>
Vector3<T> Vector3<T>::crossProduct(const Vector3<T>& v2) const {
  Vector3<T> vn;

  vn.x_ = ((this->y_ * v2.z_) - (this->z_ * v2.y_));
  vn.y_ = -((this->x_ * v2.z_) - (this->z_ * v2.x_));
  vn.z_ = ((this->x_ * v2.y_) - (this->y_ * v2.x_));

  return vn;
}

///////////////////////////////////////////Vector4/////////////////////////////////////

template<typename T>
bool operator == (const Vector4<T>& v1, const Vector4<T>& v2) {
  return vector_impl::equalZero(v1.x_ - v2.x_) && vector_impl::equalZero(v1.y_ - v2.y_) && vector_impl::equalZero(v1.z_ - v2.z_);
}

template<typename T>
inline bool operator != (const Vector4<T>& v1, const Vector4<T>& v2) {
  return !(v1 == v2);
}

template<typename T>
Vector4<T> operator + (const Vector4<T>& v1, const Vector4<T>& v2) {
  return Vector4<T>(v1.x_ + v2.x_, v1.y_ + v2.y_, v1.z_ + v2.z_);
}

template<typename T>
Vector4<T>& operator += (Vector4<T>& v1, const Vector4<T>& v2) {
  v1.x_ += v2.x_;
  v1.y_ += v2.y_;
  v1.z_ += v2.z_;
  return v1;
}

template<typename T>
Vector4<T> operator - (const Vector4<T>& v1, const Vector4<T>& v2) {
  return Vector4<T>(v1.x_ - v2.x_, v1.y_ - v2.y_, v1.z_ - v2.z_);
}

template<typename T>
Vector4<T>& operator -= (Vector4<T>& v1, const Vector4<T>& v2) {
  v1.x_ -= v2.x_;
  v1.y_ -= v2.y_;
  v1.z_ -= v2.z_;
  return v1;
}

template<typename T, typename ValueT>
Vector4<T> operator * (const Vector4<T>& v1, ValueT value) {
  return  Vector4<T>(v1.x_ * value, v1.y_ * value, v1.z_ * value);
}

template<typename T, typename ValueT>
inline Vector4<T> operator * (ValueT value, const Vector4<T>& v1) {
  return operator *(v1, value);
}

template<typename T, typename ValueT>
Vector4<T>& operator *= (Vector4<T>& v1, ValueT value) {
  v1.x_ *= value;
  v1.y_ *= value;
  v1.z_ *= value;
  return v1;
}

template<typename T>
T Vector4<T>::length() const {
  return ::sqrt(x_*x_ + y_*y_ + z_*z_);
}

template<typename T>
Vector4<T> Vector4<T>::normalize() const {
  Vector4<T> v(*this);
  const T len = this->length();
  if (vector_impl::equalZero(len)) {
    throw VectorDivideZeroException("Vector4 length is zero");
  }

  v.x_ /= len;
  v.y_ /= len;
  v.z_ /= len;
  return v;
}

template<typename T>
inline T Vector4<T>::dotProduct(const Vector4<T>& v2) const {
  return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
}

template<typename T>
inline T Vector4<T>::cos(const Vector4<T>& v2) const {
  const auto v1len = this->length();
  const auto v2len = v2.length();
  return this->dotProduct(v2) / (v1len * v2len);
}

template<typename T>
inline T Vector4<T>::angle(const Vector4<T>& v2) const {
  return ::acos(this->cos(v2));
}

template<typename T>
inline Vector4<T> Vector4<T>::projectOnto(const Vector4<T>& v2) const {
  //this->length() * this->cos(v2) * v2.normalize();
  auto norV2 = v2.normalize();
  return norV2 * this->dotProduct(norV2);
}

template<typename T>
Vector4<T> Vector4<T>::crossProduct(const Vector4<T>& v2) const {
  Vector4<T> vn;

  vn.x_ = ((this->y_ * v2.z_) - (this->z_ * v2.y_));
  vn.y_ = -((this->x_ * v2.z_) - (this->z_ * v2.x_));
  vn.z_ = ((this->x_ * v2.y_) - (this->y_ * v2.x_));

  return vn;
}

typedef Vector2<int> Vector2I;
typedef Vector3<int> Vector3I;
typedef Vector4<int> Vector4I;

typedef Vector2<float> Vector2F;
typedef Vector3<float> Vector3F;
typedef Vector4<float> Vector4F;

typedef Vector2<double> Vector2FD;
typedef Vector3<double> Vector3FD;
typedef Vector4<double> Vector4FD;
}





#endif // !S3D_MATH_VECTOR_H
