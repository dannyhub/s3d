#include "Object.h"

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

  void setCamera(Object& obj, Point4FD pt, double anglex, double angley, double anglez) {
    const auto cosx = ::cos(anglex);
    const auto sinx = ::sin(anglex);

    const auto cosy = ::cos(angley);
    const auto siny = ::sin(angley);

    const auto cosz = ::cos(anglez);
    const auto sinz = ::sin(anglez);

    Matrix4x4FD translateMat = {1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      -pt.x_, -pt.y_, -pt.z_, 1};

    //rotate with: YXZ
    Matrix4x4FD rotateYMat = {cosy, 0, -siny, 0,
      0, 1, 0, 0,
      siny, 0, cosy, 0,
      0, 0, 0, 1};

    Matrix4x4FD rotateXMat = {1, 0, 0, 0,
      0, cosx, -sinx, 0,
      0, sinx, cosx, 0,
      0, 0, 0, 1};


    Matrix4x4FD rotateZMat = {cosz, -sinz, 0, 0,
      sinz, cosz, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1};

    const auto mat = translateMat * rotateYMat  * rotateXMat * rotateZMat;
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      *transVerit = *transVerit * mat;
      ++transVerit;
    }
  }

  void projection(Object& obj, int viewWidth, int viewHeight) {
    //nearsZ = 1
    //widthHeightRate = viewWidth / viewHeight
    //px = x / z, [-1,1]
    //py = y / z, [-1,1]

    //vx = (px + 1)  * (viewWidth-1) / 2 = (px + 1)  * ((viewWidth/2) - 1/2)
    //vy = (viewHeight - 1) - (py + 1)  * (viewHeight-1) / 2 = (viewHeight - 1) - (py + 1)  * ((viewHeight/2) - 1/2)

    Matrix4x4FD pmat = {1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1};

    const auto xr = (double)viewWidth / 2. - 0.5;
    const auto yr = (double)viewHeight / 2. - 0.5;

    const auto h = viewHeight - 1;
    Matrix4x4FD vmat = {xr, 0, 0, 0,
      0, -yr, 0, 0,
      0, 0, 0, 0,
      xr, h - yr, 0, 1};

    //const auto mat = pmat * vmat;
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      const auto pt = *transVerit;
      pmat[3][3] = pt.z_;

      *transVerit = pt * pmat * vmat;
      ++transVerit;
    }
  }

}// s3d