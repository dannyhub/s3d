#include "Camera.h"


namespace s3d
{

CameraUVN::CameraUVN(const Point4FD& pos, const Point4FD& targetPos, double fovDegree,
                          double nearZ, double farZ, double screenWidth, double screenHeight) {
  fov_ = fovDegree;
  position_ = pos;
  target_ = targetPos;
  screenWidth_ = screenWidth;
  screenHeight_ = screenHeight;
  nearClipZ_ = nearZ;
  farClipZ_ = farZ;
  aspectRatio_ = screenWidth / screenHeight;

  viewPlaneHeight_ = viewPlaneWidth_ = 2.0;
  viewDistance_ = 0.5 * viewPlaneWidth_ / ::tan(degreeToRadius(fovDegree * 0.5));
  assert(viewDistance_ != 0.);

  buildUVNVector();
  //rightClipPlane_.point_ = {0,0,0};

  //const double slopRight = -0.5 * viewPlaneWidth_  / viewDistance_;
  const auto halfWidth = 0.5 * viewPlaneWidth_;

  rightClipPlane_.n_ = {viewDistance_, 0, -halfWidth};
  leftClipPlane_.n_ = {-viewDistance_, 0, -halfWidth};

  //const double slopTop = -0.5 * viewPlaneHeight_ / viewDistance_;
  const auto halfHeight = 0.5 * viewPlaneHeight_;

  topClipPlane_.n_ = {0, viewDistance_, -halfHeight};
  bottomClipPlane_.n_ = {0, -viewDistance_, -halfHeight};

  matWorldToCamera_ = buildWorldToCameraMatrix4x4FD();
  matCameraToPerspective_ = buildCameraToPerspectiveMatrix4x4FD();
  matCameraToScreen_ = buildCameraToScreenMatrix4x4FD();
  matPerspectiveToSreen_ = buildPerspectiveToScreenMatrix4x4FD();
  matWorldToScreen_ = buildWorldToSreenMatrix4x4FD();
}

CameraUVN::~CameraUVN() {
}

void CameraUVN::buildUVNVector() {
  n_ = target_ - position_;

  v_ = {0, 1, 0};
  u_ = v_.crossProduct(n_);
  v_ = n_.crossProduct(u_);

  n_.normalizeSelf();
  u_.normalizeSelf();
  v_.normalizeSelf();
}

Matrix4x4FD CameraUVN::buildWorldToCameraMatrix4x4FD() {
  Matrix4x4FD rmat   {u_.x_, v_.x_, n_.x_, 0,
                      u_.y_, v_.y_, n_.y_, 0,
                      u_.z_, v_.z_, n_.z_, 0,
                      0,0,0,1};

  Matrix4x4FD tmat   {1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      -position_.x_, -position_.y_, -position_.z_, 1};

  return rmat * tmat;
}

Matrix4x4FD CameraUVN::buildCameraToPerspectiveMatrix4x4FD() {
  /*
   *  aspectRatio_ = screenWidth_ / screenHeight_
   *  px = x *  viewDistance_ / z, [-1,1]
   *  py = aspectRatio_ * y * viewDistance_ / z, [-1,1]
   *
   */
  const auto vd = viewDistance_;
  const auto ar = aspectRatio_;
  Matrix4x4FD mat   {vd, 0,       0, 0,
                      0, vd * ar, 0, 0,
                      0, 0,       1, 1,
                      0, 0,       0, 0};
  return mat;
}

Matrix4x4FD CameraUVN::buildCameraToScreenMatrix4x4FD() {
  return buildCameraToPerspectiveMatrix4x4FD() * buildPerspectiveToScreenMatrix4x4FD();
}

Matrix4x4FD CameraUVN::buildPerspectiveToScreenMatrix4x4FD() {
  /*
  let sw = screenWidth_
  let sh = screenHeight_

  vx = (px + 1)  * (sw-1) / 2 
     = (px + 1)  * ((sw/2) - 1/2)
     = px * ((sw/2) - 1/2) + ((sw/2) - 1/2)

  vy = (sh - 1) - (py + 1)  * (sh-1) / 2 = (sh - 1) - (py + 1)  * ((sh/2) - 1/2)
     = -(py + 1)  * ((sh/2) - 1/2) + (sh - 1)
     = -py * ((sh/2) - 1/2) - ((sh/2) - 1/2) + (sh - 1)
     = -py * ((sh/2) - 1/2) + ((sh/2) - 1/2)
  */

  const auto xalpha = screenWidth_ * 0.5 - 0.5;
  const auto ybeta = screenHeight_ * 0.5 - 0.5;
    
  Matrix4x4FD mat   {xalpha, 0,     0,  0,
                      0,     -ybeta, 0,  0,
                      0,      0,     1,  0,
                      xalpha, ybeta, 0,  1};

  return mat;
}

Matrix4x4FD CameraUVN::buildWorldToSreenMatrix4x4FD() {
  return matWorldToCamera_ * matCameraToPerspective_ * matPerspectiveToSreen_;
}

bool CameraUVN::isBackface(const Point4FD& pt, const Vector4FD& normal) {
  Vector4FD vp(pt, getPosition());

  if (vp.dotProduct(normal) > 0.) {
    return false;
  } else {
    return true;
  }
}

bool CameraUVN::isSphereOutOfView(const Point4FD& position, double radius) {
  if (position.z_ - radius > farClipZ_)
    return true;

  if (position.z_ + radius < nearClipZ_)
    return true;

  //x-z plane
  {
    const double ratio = 0.5 * viewPlaneWidth_ / viewDistance_;
    const double x = position.z_ * ratio;

    //positive
    if (position.x_ - radius > x)
      return true;

    //negative
    if (position.x_ + radius < -x)
      return true;
  }

  //y-z plane
  {
    const double ratio = 0.5 * viewPlaneHeight_ / viewDistance_;
    const double y = position.z_ * ratio;

    //positive
    if (position.y_ - radius > y)
      return true;

    //negative
    if (position.y_ + radius < -y)
      return true;
  }

  return false;
}


}// s3d