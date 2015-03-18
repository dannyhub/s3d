#pragma once
#include "../math/Math.h"
#include "Polygon.h"
#include "VertexList.h"
#include "Vertex.h"

#include <string>
#include <vector>
#include <memory>
#include <boost/noncopyable.hpp>

namespace s3d
{

class Object : private boost::noncopyable {
public:
  typedef Point4<double> PointType;
  typedef Vector4<double> VectorType;

  typedef Vertex VertexType;
  typedef VertexList<VertexType> VertexListType;
  typedef Polygon<3U> PolygonType;

  typedef std::vector<PolygonType> PolygonListType;

  typedef std::vector<PolygonType>::iterator iterator;
  typedef std::vector<PolygonType>::const_iterator const_iterator;
  typedef std::vector<PolygonType>::size_type size_type;

  Object() : id_(0), direction_(0, 0, 0.f) {
  }
  
  Object(int id, const std::string& name) : id_(id), name_(name), direction_(0, 0, 1.f){
  }

  ~Object() {
  }     

  void addPolygon(const PolygonType& p) {
    polygons_.push_back(p);
  }

   //FIXME
  //void addPolygon(std::initializer_list<PolygonType>& l) {
  //  for(auto& p : l) {
  //    polygons_.push_back(p);
  //  }
  //}

  void setWorldPosition(const PointType& pt) {
    worldPosition_ = pt;
  }

  iterator begin() {
    assert(!polygons_.empty());
    return polygons_.begin();
  }

  iterator end() {
    return polygons_.end();
  }

  const_iterator begin() const {
    assert(!polygons_.empty());
    return polygons_.begin();
  }

  const_iterator end() const {
    return polygons_.end();
  }

  size_type size() const {
    return polygons_.size();
  }

  PolygonListType& getPolygonList() {
    return polygons_;
  }

private:
  int id_;
  std::string name_;

  float maxRadius_;
  float averageRadius_;

  VectorType direction_;
  VectorType ux_, uy_, yz_;

  PolygonListType polygons_;
  PointType worldPosition_;

};

typedef std::shared_ptr<Object> ObjectPtr;

//Matrix4x4FD buildRotateMatrix4x4(double anglex, double angley, double anglez);
//
//void addToWorld(Object& obj, double x, double y, double z);
//
//void backFaceRemove(Object& obj, const Point4FD& viewLine, double farZ = 1000);
//
//void setCamera(Object& obj, const Point4FD& pt, double anglex, double angley, double anglez);
//
////perspective projection with viewing distance 1 and viewing angle 90
//void perspectiveProject(Object& obj, double viewWidth, double viewHeight);
//
//void perspectiveProject(Object& obj, double fieldOfViewDegree, double viewWidth, double viewHeight);


}// s3d