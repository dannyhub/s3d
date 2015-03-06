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
  typedef Point4<double> VectorType;

  World();
  ~World();

  void addToWorld(ObjectPtr obj, const Point3FD& pos);
  VertexList<PointType> worldVertices;

private:
  //VertexList<Point3<float>> worldVertices;
 
  std::vector<ObjectPtr> objects_;
};

}// namespace s3d