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

#include <boost/noncopyable.hpp>

namespace s3d
{


class CameraUVN : private boost::noncopyable {
public:
  typedef Plane3D<double> Plane3DType;

  virtual ~CameraUVN();

  Matrix4x4FD getWorldToCameraMatrix4x4FD() {
    return matWorldToCamera_;
  }

  Matrix4x4FD getCameraToProjectMatrix4x4FD() {
    return matCameraToPerspective_;
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

protected:
  CameraUVN();

  Matrix4x4FD buildWorldToCameraMatrix4x4FD();
  Matrix4x4FD buildCameraToPerspectiveMatrix4x4FD();
  Matrix4x4FD buildPerspectiveToScreenMatrix4x4FD();
  Matrix4x4FD buildWorldToSreenMatrix4x4FD();

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
  Matrix4x4FD matPerspectiveToSreen_;

  Matrix4x4FD matWorldToScreen_;

private:
  friend
  std::shared_ptr<CameraUVN> createUVNCamera(const Point4FD& pos, const Point4FD& targetPos, double fovDegree,
                                             double nearZ, double farZ, double screenWidth, double screenHeight);
};

typedef std::shared_ptr<CameraUVN> CameraPtr;

CameraPtr createUVNCamera(const Point4FD& pos, const Point4FD& targetPos, double fov,
                          double nearZ, double farZ, double screenWidth, double screenHeight);

}// s3d


