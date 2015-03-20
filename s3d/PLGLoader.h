#pragma once
#include "VertexList.h"
#include "Polygon.h"

namespace s3d
{

class PLGLoader {
public:
  PLGLoader();
  ~PLGLoader();

  void parse(const std::string& filename, std::string& name, std::vector<Point4<float>>& vlist, std::vector<Polygon<3>>& polys, float scale);

};

class PLGLoaderException : public std::exception {
public:
  PLGLoaderException(const char * const & msg) : exception(msg) {
  }
};


}// s3d     