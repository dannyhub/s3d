#include "Camera.h"


namespace s3d
{

CameraUVN::CameraUVN() {
}


CameraUVN::~CameraUVN() {
}

Matrix4x4FD CameraUVN::buildCameraToScreenMatrix4x4FD() {
  /*
  aspectRatio_ = screenWidth_ / screenHeight_
  px = x *  viewDistance_ / z, [-1,1]
  py = aspectRatio_ * y * viewDistance_ / z, [-1,1]

  vx = (px + 1)  * (screenWidth_-1) / 2 
     = (px + 1)  * ((screenWidth_/2) - 1/2)
     = px * ((screenWidth_/2) - 1/2) + ((screenWidth_/2) - 1/2)

  vy = (screenHeight_ - 1) - (py + 1)  * (screenHeight_-1) / 2 = (screenHeight_ - 1) - (py + 1)  * ((screenHeight_/2) - 1/2)
     = -(py + 1)  * ((screenHeight_/2) - 1/2) + (screenHeight_ - 1)
     = -py * ((viewHeight/2) - 1/2) - ((screenHeight_/2) - 1/2) + (screenHeight_ - 1)
     = -py * ((screenHeight_/2) - 1/2) + ((screenHeight_/2) - 1/2)
  */

  Matrix4x4FD pmat = {viewDistance_, 0, 0, 0,
                      0, viewDistance_* aspectRatio_, 0, 0,
                      0, 0, 1, 1,
                      0, 0, 0, 0};

  const auto xalpha = screenWidth_ * 0.5 - 0.5;
  const auto ybeta = screenHeight_ * 0.5 - 0.5;
    
  Matrix4x4FD vmat = {xalpha, 0,     0,  0,
                      0,     -ybeta, 0,  0,
                      0,      0,     1,  0,
                      xalpha, ybeta, 0,  1};

  return pmat * vmat;
}


Matrix4x4FD CameraUVN::buildWorldToCameraMatrix4x4FD() {
  Matrix4x4FD mat = {n_.x_, v_.x_, u_.x_, 0,
                     n_.y_, v_.y_, u_.y_, 0,
                     n_.z_, v_.z_, u_.z_, 0,
                     0,0,0,1};

  return mat;
}

bool CameraUVN::isBackFace(const Vector4FD& n) {
  return u_.dotProduct(n) > 0.;
}

bool CameraUVN::isOutOfView(const Point3FD& position, double radius) {
  if (position.z_ - radius > farZ_)
    return true;

  if (position.z_ + radius < nearZ_)
    return true;

  //x-z plane
  {
    const double ratio = 0.5 * viewWidth_ / viewDistance_;
    const double x = position.z_ * ratio;
    const double z = position.x_ / ratio;

    //positive
    if (position.x_ + radius > x && position.z_ + radius > z)
      return true;

    //negative
    if (position.x_ - radius < -x && position.z_ + radius > z)
      return true;
  }

  //y-z plane
  {
    const double ratio = 0.5 * viewHeight_ / viewDistance_;
    const double y = position.z_ * ratio;
    const double z = position.x_ / ratio;

    //positive
    if (position.y_ + radius > y && position.z_ + radius > z)
      return true;

    //negative
    if (position.y_ - radius < -y && position.z_ + radius > z)
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
  cm->nearZ_ = nearZ;
  cm->farZ_ = farZ;
  cm->aspectRatio_ = screenWidth / screenHeight;

  cm->viewHeight_ = cm->viewWidth_ = 2.0;
  cm->viewDistance_ = 0.5 * cm->viewWidth_ / ::tan(degreeToRadius(fovDegree * 0.5));
  cm->u_ = targetPos - pos;
  cm->u_.normalizeSelf();
  cm->v_ = {0, 1, 0};
  cm->n_ = cm->u_.crossProduct(cm->v_);

  cm->n_.normalizeSelf();
  cm->v_ = cm->u_.crossProduct(cm->n_);

  //cm->rightPlane_.point_ = {0,0,0};

  const double slopRight = -0.5 * cm->viewWidth_  / cm->viewDistance_;
  cm->rightPlane_.n_ = {-1, 0, slopRight};
  cm->leftPlane_.n_ = {1, 0, slopRight};

  const double slopTop = -0.5 * cm->viewHeight_ / cm->viewDistance_;
  cm->topPlane_.n_ = {0, -1, slopTop};
  cm->bottomPlane_.n_ = {0, 1, slopTop};

  return cm;
}


}// s3d