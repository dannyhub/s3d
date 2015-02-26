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

  Matrix4x4FD buildWorldToCameraMatrix4x4FD();
  Matrix4x4FD buildCameraToScreenMatrix4x4FD();

  Matrix4x4FD buildWorldToScreenMatrix4x4FD() {
    return buildWorldToCameraMatrix4x4FD() * buildCameraToScreenMatrix4x4FD();
  }

  bool isOutOfView(const Point3FD& position, double radius);
  bool isBackFace(const Vector4FD& n);

protected:
  CameraUVN();

protected:
  double fov_;              //field of view
  double viewDistance_;
  double viewWidth_;
  double viewHeight_;

  Point4FD position_;
  Vector4FD target_;
  Vector4FD u_;
  Vector4FD v_;
  Vector4FD n_;

  double nearZ_;
  double farZ_;

  double aspectRatio_;
  double screenWidth_;
  double screenHeight_;

  Plane3DType leftPlane_;
  Plane3DType rightPlane_;
  Plane3DType topPlane_;
  Plane3DType bottomPlane_;

private:
  friend
  std::shared_ptr<CameraUVN> createUVNCamera(const Point4FD& pos, const Point4FD& targetPos, double fovDegree,
                                             double nearZ, double farZ, double screenWidth, double screenHeight);
};

typedef std::shared_ptr<CameraUVN> CameraPtr;

CameraPtr createUVNCamera(const Point4FD& pos, const Point4FD& targetPos, double fov,
                          double nearZ, double farZ, double screenWidth, double screenHeight);

}// s3d

