#pragma once
#include "VertexList.h"
#include "Polygon.h"

namespace s3d
{

class PLGLoader {
public:
  PLGLoader();
  ~PLGLoader();

  void parse(const std::string& filename, std::string& name, VertexList<Point4<double>>& vlist, std::vector<Polygon<3>>& polys);

};

class PLGLoaderException : public std::exception {
public:
  PLGLoaderException(const char * const & msg) : exception(msg) {
  }
};


}// s3d