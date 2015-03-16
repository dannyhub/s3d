#include "Object.h"
#include "math/Math.h"

namespace s3d
{


  void addToWorld(Object& obj, double x, double y, double z) {
    Matrix4x4FD translateMat = {1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                x, y, z, 1};

    obj.setWorldPosition({x, y, z});
    obj.transVertexList_.clear();
    auto it = obj.localVertexList_.begin();
    while (it != obj.localVertexList_.end()) {
      const auto pt4 = *it;
      obj.transVertexList_.push_back(pt4 * translateMat);
      ++it;
    }
  }

  void backFaceRemove(Object& obj, const Point4FD& /*viewLine*/, double /*farZ*/) {
    for (auto itp : obj.polygons_) {
      //if (itp.normal_.dotProduct(viewLine) > 0.) {
        obj.transPolygons_.push_back(itp);
      //}
    }
  }

  Matrix4x4FD buildRotateMatrix4x4(double anglex, double angley, double anglez) {
    const auto cosx = ::cos(anglex);
    const auto sinx = ::sin(anglex);

    const auto cosy = ::cos(angley);
    const auto siny = ::sin(angley);

    const auto cosz = ::cos(anglez);
    const auto sinz = ::sin(anglez);

    //rotate with: YXZ
    Matrix4x4FD rotateYMat   {cosy, 0, -siny, 0,
                              0,    1,  0,    0,
                              siny, 0,  cosy, 0,
                              0,    0,   0,   1};

    Matrix4x4FD rotateXMat   {1, 0,     0,    0,
                              0, cosx, -sinx, 0,
                              0, sinx,  cosx, 0,
                              0, 0,     0,    1};


    Matrix4x4FD rotateZMat   {cosz, -sinz, 0, 0,
                              sinz,  cosz, 0, 0,
                              0,     0,    1, 0,
                              0,     0,    0, 1};

   return rotateYMat  * rotateXMat * rotateZMat;
  }

  void setCamera(Object& obj, const Point4FD& pt, double anglex, double angley, double anglez) {
    const auto cosx = ::cos(anglex);
    const auto sinx = ::sin(anglex);

    const auto cosy = ::cos(angley);
    const auto siny = ::sin(angley);

    const auto cosz = ::cos(anglez);
    const auto sinz = ::sin(anglez);

    auto viewLine = pt ;
    viewLine.z_ = 1;
    viewLine.normalizeSelf();
    //y
    double x = viewLine.x_ * cos(angley) - viewLine.z_ * sin(angley);
    double z = viewLine.x_ * sin(angley) + viewLine.z_ * cos(angley);
    viewLine.x_ = x;
    viewLine.z_ = z;

    //x
    double y = viewLine.y_ * cos(anglex) - viewLine.z_ * sin(anglex);
    z = viewLine.y_ * sin(anglex) + viewLine.z_ * cos(anglex);
    viewLine.y_ = y;
    viewLine.z_ = z;

    //z
    x = viewLine.x_ * cos(anglez) - viewLine.y_ * sin(anglez);
    y = viewLine.x_ * sin(anglez) + viewLine.y_ * cos(anglez);
    viewLine.x_ = x;
    viewLine.y_ = y;
    backFaceRemove(obj, viewLine);

    Matrix4x4FD translateMat   {1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                -pt.x_, -pt.y_, -pt.z_, 1};

    //rotate with: YXZ
    Matrix4x4FD rotateYMat   {cosy, 0, -siny, 0,
                              0,    1,  0,    0,
                              siny, 0,  cosy, 0,
                              0,    0,   0,   1};

    Matrix4x4FD rotateXMat   {1, 0,     0,    0,
                              0, cosx, -sinx, 0,
                              0, sinx,  cosx, 0,
                              0, 0,     0,    1};


    Matrix4x4FD rotateZMat   {cosz, -sinz, 0, 0,
                              sinz,  cosz, 0, 0,
                              0,     0,    1, 0,
                              0,     0,    0, 1};

    const auto mat = translateMat * rotateYMat  * rotateXMat * rotateZMat;
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      *transVerit = *transVerit * mat;
      ++transVerit;
    }
  }

  void perspectiveProject(Object& obj, double viewWidth, double viewHeight) {
    //viewing distance = 1
    //widthHeighRatio = viewWidth / viewHeight
    //px = x / z, [-1,1]
    //py = widthHeighRatio * y / z, [-1,1]

    //vx = (px + 1)  * (viewWidth-1) / 2 
    //   = (px + 1)  * ((viewWidth/2) - 1/2)

    //vy = (viewHeight - 1) - (py + 1)  * (viewHeight-1) / 2 = (viewHeight - 1) - (py + 1)  * ((viewHeight/2) - 1/2)
    //   = -(py + 1)  * ((viewHeight/2) - 1/2) + (viewHeight - 1)
    //   = -py * ((viewHeight/2) - 1/2) - ((viewHeight/2) - 1/2) + (viewHeight - 1)
    //   = -py * ((viewHeight/2) - 1/2) + ((viewHeight/2) - 1/2)
    const auto widthHeighRatio = viewWidth / viewHeight;
    Matrix4x4FD pmat   {1, 0, 0, 0,
                        0, widthHeighRatio, 0, 0,
                        0, 0, 1, 1,
                        0, 0, 0, 0};

    const auto xalpha = viewWidth * 0.5 - 0.5;
    const auto ybeta = viewHeight * 0.5 - 0.5;
    
    Matrix4x4FD vmat   {xalpha, 0,     0,  0,
                        0,     -ybeta, 0,  0,
                        0,      0,     1,  0,
                        xalpha, ybeta, 0,  1};

    const auto mat = pmat * vmat;
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      const auto pt = *transVerit;
      *transVerit = pt * mat;
      ++transVerit;
    }
  }

  void perspectiveProject(Object& obj, double fieldOfViewDegree, double viewWidth, double viewHeight) {
    const double viewingDistance = viewWidth * 0.5  / ::tan(degreeToRadius(fieldOfViewDegree / 2.));

    Matrix4x4FD pmat   {viewingDistance, 0,               0, 0,
                        0,               viewingDistance, 0, 0,
                        0,               0,               1, 1.,
                        0,               0,               0, 0};

    Matrix4x4FD vmat   {1, 0,    0, 0,
                        0, -1,   0, 0,
                        0, 0,    1, 0,
                        viewWidth * 0.5, (viewHeight - 1) - viewHeight*0.5, 0, 1};

    const auto mat = pmat * vmat;
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      const auto pt = *transVerit;
      *transVerit = pt * mat;
      ++transVerit;
    }
  }
}// s3d