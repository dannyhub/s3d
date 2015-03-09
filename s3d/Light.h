#pragma once
#include "math/Math.h"
#include "Color.h"
#include <boost/noncopyable.hpp>

#include <cassert>

namespace s3d
{

class LightI {
public:
  virtual Color computeIntensity() = 0;
};

class AmbientLight {
public:
  AmbientLight(const Color& color) : color_(color){}
  ~AmbientLight() {}

  virtual Color computeIntensity() {
    return color_;
  }

private:
  Color color_;
};


class InfiniteLight {
public:
  InfiniteLight(const Color& color, const Vector4FD& dir) : color_(color), dir_(dir) {}
  ~InfiniteLight() {}

  virtual Color computeIntensity(const Vector4FD& planeN) {
    assert(planeN.isNormalize());
    assert(dir_.isNormalize());

    auto cosv = planeN.dotProduct(dir_);
    if (cosv <= 0.)
      return Color(0);


    return color_ * cosv;
  }

private:
  Color color_;
  Vector4FD dir_;
};

class PointLight {
public:
  PointLight(const Point4FD& pos, const Color& color, double intensity, double kc = 0. , double kl = 1.0 , double kq = 1.) 
    : pos_(pos),
      color_(color), 
      intensity_(intensity),
      kc_(kc), kl_(kl), kq_(kq) {
  }

  ~PointLight() {}

  virtual Color computeIntensity(const Point4FD& destPt, const Vector4FD& planeN) {
    assert(planeN.isNormalize());
    Vector4FD lightDir(destPt, pos_);
    const auto ldist = lightDir.length();
    lightDir.normalizeSelf();

    auto df = planeN.dotProduct(lightDir);
    if (df <= 0.)
      return Color(0);

    return color_ * (df / (kc_ + kl_*ldist + kq_*ldist*ldist));
  }

private:
  Point4FD pos_;
  Color color_;
  double intensity_;
  double kc_, kl_, kq_;

};

class SpotLight {
public:
  SpotLight(const Point4FD& pos, const Vector4FD& dir, double innerAngle, double outerAngle, const Color& color,
            double kc = 0., double kl = 1., double kq = 1., double pf = 1.)
    : pos_(pos),
      dir_(dir),
      innerAngle_(innerAngle),
      outerAngle_(outerAngle),
      color_(color),
      kc_(kc), kl_(kl), kq_(kq), pf_(pf) {
  }

  ~SpotLight() {}

  virtual Color computeIntensity(const Point4FD& destPt, const Vector4FD& planeN) {
    assert(planeN.isNormalize());
    const auto df = planeN.dotProduct(dir_);
    if (df <= 0.)
      return Color(0);

    Vector4FD lightDir(destPt, pos_);
    const auto ldist = lightDir.length();
    lightDir.normalizeSelf();

    auto cosv = lightDir.dotProduct(dir_);
    const auto angle = ::acos(cosv);
    if (cosv < 0. || angle > outerAngle_) {
      return Color(0);
    }

    if (angle < innerAngle_) {
      return color_ * (df / (kc_ + kl_*ldist + kq_*ldist*ldist));
    } else {
      const auto ff = ::pow((::cos(angle) - ::cos(outerAngle_*0.5)), pf_) / (::cos(innerAngle_*0.5) - ::cos(outerAngle_*0.5));
      return color_ * (df / (kc_ + kl_*ldist + kq_*ldist*ldist) * ff);
    }
  }

private:
  Point4FD pos_;
  Vector4FD dir_;
  Color color_;
  double innerAngle_;
  double outerAngle_;
  double kc_, kl_, kq_, pf_;

};


}// s3d