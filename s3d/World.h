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
  typedef Point4<double> PointType;
  typedef Object::VertexListType VertexListType;
  typedef Point4<double> VectorType;

  typedef std::vector<ObjectPtr>::iterator iterator;
  typedef std::vector<ObjectPtr>::const_iterator const_iterator;
  typedef std::vector<ObjectPtr>::size_type size_type;

  typedef VertexListType::iterator vertexlist_iterator;
  typedef VertexListType::const_iterator vertexlist_const_iterator;

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

  vertexlist_iterator vertexlist_begin() {
    return worldVertices.begin();
  }

  vertexlist_iterator vertexlist_end() {
    return worldVertices.end();
  }

  vertexlist_const_iterator vertexlist_begin() const {
    return worldVertices.begin();
  }

  vertexlist_const_iterator vertexlist_end() const {
    return worldVertices.end();
  }

  size_type vertexlist_size() const {
    return worldVertices.size();
  }

  VertexListType& getworldVertices() {
    return worldVertices;
  }

private:
  VertexListType worldVertices;
  std::vector<ObjectPtr> objects_;
};

}// namespace s3d