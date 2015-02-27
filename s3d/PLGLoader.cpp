#include "PLGLoader.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace s3d
{


PLGLoader::PLGLoader() {
}


PLGLoader::~PLGLoader() {
}

void PLGLoader::parse(const std::string& filename, std::string& name, VertexList<Point4<double>>& vlist, std::vector<Polygon<3>>& polys) {
  fstream fin;
  fin.open(filename.c_str(), ios_base::in);
  if (!fin) 
    throw PLGLoaderException("file open failed");

  char cname[256] = {0};
  int num_verts = 0;
  int num_polys = 0;

  string linestr;
  while (std::getline(fin, linestr)) {
//    assert(!linestr.empty());
    if (!linestr.empty()) {
      if (*linestr.begin() == '#')
        continue;

      sscanf_s(linestr.c_str(), "%s %d %d", cname, 256 ,& num_verts, &num_polys);
      assert(strlen(cname) > 0 && num_verts > 0 && num_polys > 0);
      break;
    }
    linestr.clear();
  }

  if (strlen(cname) == 0 || num_verts <= 0 || num_polys <= 0)
    throw PLGLoaderException("strlen(name) == 0 || num_verts <= 0 || num_polys <= 0");

  name = cname;
  //vertex list
  int vertsRead = 0;
  linestr.clear();
  while (std::getline(fin, linestr)) {
    //assert(!linestr.empty());
    if (!linestr.empty()) {
      if (*linestr.begin() == '#')
        continue;

      int v1,v2,v3;

      sscanf_s(linestr.c_str(), "%d %d %d", &v1, &v2, &v3);
      vlist.push_back({(double)v1, (double)v2, (double)-v3});
      if (++vertsRead == num_verts)
        break;
    }
    linestr.clear();
  }

  if (vertsRead != num_verts)
    throw PLGLoaderException("vertsRead != num_verts");

  //vertex list
  int polysRead = 0;
  linestr.clear();
  while (std::getline(fin, linestr)) {
    //assert(!linestr.empty());
    if (!linestr.empty()) {
      if (*linestr.begin() == '#')
        continue;

      unsigned int v1, v2, v3;
      int flag = 0;
      unsigned int num = 0;
      sscanf_s(linestr.c_str(), "%X %d %d %d %d", &flag, &num, &v1, &v2, &v3);

      Polygon<3> poly = {v3, v2, v1};

      if (flag & 0x1000 )
        poly.setState(kPolygonState2Side);

      polys.push_back(poly);
      if (++polysRead == num_polys)
        break;
    }
    linestr.clear();
  }

  if (polysRead != num_polys)
    throw PLGLoaderException("vertsRead != num_verts");
}

}// s3d