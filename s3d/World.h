#pragma once
#include "math/Math.h"

#include "VertexList.h"
#include "Polygon.h"
#include "Object.h"

#include <vector>

namespace s3d
{

class World {
public:
  typedef std::vector<Object::PolygonType> PolygonListType;
  typedef Object::VertexListType VertexListType;
  typedef Point4<double> VectorType;
  typedef Point4<double> PointType;

  typedef std::vector<ObjectPtr>::iterator iterator;
  typedef std::vector<ObjectPtr>::const_iterator const_iterator;
  typedef std::vector<ObjectPtr>::size_type size_type;

  World();
  ~World();

  void addToWorld(ObjectPtr obj, const Point3FD& pos);

  iterator begin() {
    return objects_.begin();
  }

  iterator end() {
    return objects_.end();
  }

  const_iterator begin() const {
    return objects_.begin();
  }

  const_iterator end() const {
    return objects_.end();
  }

  size_type size() const {
    return objects_.size();
  }

  VertexListType& getWorldVertices() {
    return worldVertices;
  }

  PolygonListType& getPolygonList() {
    return polys_;
  }

private:
  VertexListType worldVertices;
  std::vector<ObjectPtr> objects_;
  PolygonListType polys_;
};

}// namespace s3d