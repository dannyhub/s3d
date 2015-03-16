#pragma once
#include "../math/Math.h"
#include "Polygon.h"
#include "VertexList.h"
#include "Vertex.h"

#include <string>
#include <vector>
#include <memory>

namespace s3d
{

class Object {
public:
  typedef Point4<double> PointType;
  typedef Point4<double> VectorType;

  typedef Vertex VertexType;
  typedef VertexList<VertexType> VertexListType;
  typedef Polygon<3U> PolygonType;

  Object() : id_(0), direction_(0, 0, 0.f) {
  }
  
  Object(int id, const std::string& name) : id_(id), name_(name), direction_(0, 0, 1.f){
  }

  ~Object() {
  }     

  void addVertex(const VertexType& pt) {
    localVertexList_.push_back(pt);
  }

  void addPolygon(const PolygonType& p) {
    //p.setVertexList();
    polygons_.push_back(p);
  }

  void setWorldPosition(const PointType& pt) {
    worldPosition_ = pt;
  }

  VertexListType localVertexList_;
  VertexListType transVertexList_;

  std::vector<PolygonType> polygons_;
  std::vector<PolygonType> transPolygons_;

  PointType worldPosition_;

private:
  int id_;
  std::string name_;

  float maxRadius_;
  float averageRadius_;

 
  VectorType direction_;

  VectorType ux_, uy_, yz_;

};

typedef std::shared_ptr<Object> ObjectPtr;

Matrix4x4FD buildRotateMatrix4x4(double anglex, double angley, double anglez);

void addToWorld(Object& obj, double x, double y, double z);

void backFaceRemove(Object& obj, const Point4FD& viewLine, double farZ = 1000);

void setCamera(Object& obj, const Point4FD& pt, double anglex, double angley, double anglez);

//perspective projection with viewing distance 1 and viewing angle 90
void perspectiveProject(Object& obj, double viewWidth, double viewHeight);

void perspectiveProject(Object& obj, double fieldOfViewDegree, double viewWidth, double viewHeight);


}// s3d