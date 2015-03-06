#pragma once
#include "math/Math.h"
#include "math/Point.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "Polygon.h"
#include "VertexList.h"
#include "math/Geometry.h"

#include <string>
#include <vector>
#include <memory>

namespace s3d
{

class CameraUVN {
public:
  typedef Plane3D<double> Plane3DType;

  CameraUVN(const Point4FD& pos, const Point4FD& targetPos, double fovDegree,
            double nearZ, double farZ, double screenWidth, double screenHeight);

  virtual ~CameraUVN();

  Matrix4x4FD getWorldToCameraMatrix4x4FD() {
    return matWorldToCamera_;
  }

  Matrix4x4FD getCameraToProjectMatrix4x4FD() {
    return matCameraToPerspective_;
  }

  Matrix4x4FD getCameraToScreenMatrix4x4FD() {
    return matCameraToScreen_;
  }
  
  Matrix4x4FD getPerspectiveToScreenMatrix4x4FD() {
    return matPerspectiveToSreen_;
  }

  Matrix4x4FD getWorldToScreenMatrix4x4FD() {
    return matWorldToScreen_;
  }

  bool isSphereOutOfView(const Point4FD& position, double radius);
  bool isBackFacePlane(const Vector4FD& n);

  Point4FD getPosition() const {
    return position_;
  }

  Vector4FD getU() const {
    return u_;
  }

  Vector4FD getV() const {
    return v_;
  }

  Vector4FD getN() const {
    return n_;
  }

protected:
  Matrix4x4FD buildWorldToCameraMatrix4x4FD();
  Matrix4x4FD buildCameraToPerspectiveMatrix4x4FD();
  Matrix4x4FD buildCameraToScreenMatrix4x4FD();
  Matrix4x4FD buildPerspectiveToScreenMatrix4x4FD();
  Matrix4x4FD buildWorldToSreenMatrix4x4FD();

  void buildUVNVector();

protected:
  double fov_;              //field of view
  double viewDistance_;
  double viewPlaneWidth_;
  double viewPlaneHeight_;

  Point4FD position_;
  Vector4FD target_;
  Vector4FD u_;
  Vector4FD v_;
  Vector4FD n_;

  double nearClipZ_;
  double farClipZ_;

  double aspectRatio_;
  double screenWidth_;
  double screenHeight_;

  Plane3DType leftClipPlane_;
  Plane3DType rightClipPlane_;
  Plane3DType topClipPlane_;
  Plane3DType bottomClipPlane_;

  Matrix4x4FD matWorldToCamera_;
  Matrix4x4FD matCameraToPerspective_;
  Matrix4x4FD matCameraToScreen_;
  Matrix4x4FD matPerspectiveToSreen_;

  Matrix4x4FD matWorldToScreen_;
};

typedef std::shared_ptr<CameraUVN> CameraPtr;


}// s3d


