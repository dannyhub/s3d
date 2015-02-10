#pragma once
#include "math/Point.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "Polygon.h"
#include "VertexList.h"

#include <string>
#include <vector>

namespace s3d
{

class Object {
public:
  typedef Point4<double> PointType;
  typedef Point4<double> VectorType;

  typedef VertexList<PointType> VertexListType;
  typedef Polygon<3U> PolygonType;

  Object(int id, const std::string& name) : id_(id), name_(name), direction_(0, 0, 1.f){
  }

  ~Object() {
  }
  

/*  void addVertexList(const std::initializer_list<PointType>& ilist) {
    std::for_each(ilist.begin(), ilist.end(), [&] -> (const PointType& pt) {
      this->addVertex(pt);
    });
  }    */        

  void addVertex(const PointType& pt) {
    localVertexList_.push_back(pt);
  }

  void addPolygon(const PolygonType& p) {
    polygons_.push_back(p);
  }

  void setWorldPosition(const PointType& pt) {
    worldPosition_ = pt;
  }

  VertexListType localVertexList_;
  VertexListType transVertexList_;

private:
  int id_;
  std::string name_;

  float maxRadius_;
  float averageRadius_;

  PointType worldPosition_;
  VectorType direction_;

  VectorType ux_, uy_, yz_;
  std::vector<PolygonType> polygons_;

};

template<typename T>
inline Point4<T> operator * (Point4<T> pt4, const Matrix<T, 4U, 4U>& m1) {
  Point4<T> ptRes;
  ptRes.x_ = m1[0][0] * pt4.x_ + m1[1][0] * pt4.y_ + m1[2][0] * pt4.z_ + m1[3][0] * pt4.w_;
  ptRes.y_ = m1[0][1] * pt4.x_ + m1[1][1] * pt4.y_ + m1[2][1] * pt4.z_ + m1[3][1] * pt4.w_;
  ptRes.z_ = m1[0][2] * pt4.x_ + m1[1][2] * pt4.y_ + m1[2][2] * pt4.z_ + m1[3][2] * pt4.w_;
  ptRes.w_ = m1[0][3] * pt4.x_ + m1[1][3] * pt4.y_ + m1[2][3] * pt4.z_ + m1[3][3] * pt4.w_;
  assert(ptRes.w_ != 0.f);

  if (ptRes.w_ != 1.f) {
    ptRes.x_ /= ptRes.w_;
    ptRes.y_ /= ptRes.w_;
    ptRes.z_ /= ptRes.w_;
    ptRes.w_ = 1.f;
  }

  return ptRes;
}

void addToWorld(Object& obj, double x, double y, double z);

void setCamera(Object& obj, Point4FD pt, double anglex, double angley, double anglez);

void projection(Object& obj, int viewWidth, int viewHeight);
}// s3d