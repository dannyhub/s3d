#include "Camera.h"


namespace s3d
{

CameraUVN::CameraUVN() {
}


CameraUVN::~CameraUVN() {
}

Matrix4x4FD CameraUVN::buildWorldToCameraMatrix4x4FD() {
  Matrix4x4FD rmat = {u_.x_, v_.x_, n_.x_, 0,
                      u_.y_, v_.y_, n_.y_, 0,
                      u_.z_, v_.z_, n_.z_, 0,
                      0,0,0,1};

  Matrix4x4FD tmat = {1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      -position_.x_, -position_.y_, position_.z_, 1};

  return rmat * tmat;
}

Matrix4x4FD CameraUVN::buildCameraToPerspectiveMatrix4x4FD() {
  /*
   *  aspectRatio_ = screenWidth_ / screenHeight_
   *  px = x *  viewDistance_ / z, [-1,1]
   *  py = aspectRatio_ * y * viewDistance_ / z, [-1,1]
   *
   */

  Matrix4x4FD mat = {viewDistance_, 0, 0, 0,
                      0, viewDistance_* aspectRatio_, 0, 0,
                      0, 0, 1, 1,
                      0, 0, 0, 0};
  return mat;
}

Matrix4x4FD CameraUVN::buildPerspectiveToScreenMatrix4x4FD() {
  /*
  vx = (px + 1)  * (screenWidth_-1) / 2 
     = (px + 1)  * ((screenWidth_/2) - 1/2)
     = px * ((screenWidth_/2) - 1/2) + ((screenWidth_/2) - 1/2)

  vy = (screenHeight_ - 1) - (py + 1)  * (screenHeight_-1) / 2 = (screenHeight_ - 1) - (py + 1)  * ((screenHeight_/2) - 1/2)
     = -(py + 1)  * ((screenHeight_/2) - 1/2) + (screenHeight_ - 1)
     = -py * ((viewHeight/2) - 1/2) - ((screenHeight_/2) - 1/2) + (screenHeight_ - 1)
     = -py * ((screenHeight_/2) - 1/2) + ((screenHeight_/2) - 1/2)
  */

  const auto xalpha = screenWidth_ * 0.5 - 0.5;
  const auto ybeta = screenHeight_ * 0.5 - 0.5;
    
  Matrix4x4FD mat = {xalpha, 0,     0,  0,
                      0,     -ybeta, 0,  0,
                      0,      0,     1,  0,
                      xalpha, ybeta, 0,  1};

  return mat;
}

Matrix4x4FD CameraUVN::buildWorldToSreenMatrix4x4FD() {
  return matWorldToCamera_ * matCameraToPerspective_ * matPerspectiveToSreen_;
}

bool CameraUVN::isBackFacePlane(const Vector4FD& n) {
  Vector4FD vp = position_ - target_;
  return vp.dotProduct(n) < 0.;
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

CameraPtr createUVNCamera(const Point4FD& pos, const Point4FD& targetPos, double fovDegree,
                          double nearZ, double farZ, double screenWidth, double screenHeight) {
  CameraPtr cm(new CameraUVN);
  cm->fov_ = fovDegree;
  cm->position_ = pos;
  cm->target_ = targetPos;
  cm->screenWidth_ = screenWidth;
  cm->screenHeight_ = screenHeight;
  cm->nearClipZ_ = nearZ;
  cm->farClipZ_ = farZ;
  cm->aspectRatio_ = screenWidth / screenHeight;

  cm->viewPlaneHeight_ = cm->viewPlaneWidth_ = 2.0;
  cm->viewDistance_ = 0.5 * cm->viewPlaneWidth_ / ::tan(degreeToRadius(fovDegree * 0.5));
  cm->n_ = targetPos - pos;
  
  cm->v_ = {0, 1, 0};
  cm->u_ = cm->v_.crossProduct(cm->n_);
  cm->v_ = cm->n_.crossProduct(cm->u_);

  cm->n_.normalizeSelf();
  cm->u_.normalizeSelf();
  cm->v_.normalizeSelf();
  //cm->rightClipPlane_.point_ = {0,0,0};

  //const double slopRight = -0.5 * cm->viewPlaneWidth_  / cm->viewDistance_;
  const auto halfWidth = 0.5 * cm->viewPlaneWidth_;

  cm->rightClipPlane_.n_ = {cm->viewDistance_, 0, -halfWidth};
  cm->leftClipPlane_.n_ = {-cm->viewDistance_, 0, -halfWidth};

  //const double slopTop = -0.5 * cm->viewPlaneHeight_ / cm->viewDistance_;
  const auto halfHeight = 0.5 * cm->viewPlaneHeight_;

  cm->topClipPlane_.n_ = {0, cm->viewDistance_, -halfHeight};
  cm->bottomClipPlane_.n_ = {0, -cm->viewDistance_, -halfHeight};

  cm->matWorldToCamera_ = cm->buildWorldToCameraMatrix4x4FD();
  cm->matCameraToPerspective_ = cm->buildCameraToPerspectiveMatrix4x4FD();
  cm->matPerspectiveToSreen_ = cm->buildPerspectiveToScreenMatrix4x4FD();
  cm->matWorldToScreen_ = cm->buildWorldToSreenMatrix4x4FD();
  return cm;
}


}// s3d