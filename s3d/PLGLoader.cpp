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

void PLGLoader::parse(const std::string& filename, std::string& name, VertexList<Point4<double>>& vlist, std::vector<Polygon<3>>& polys, float scale) {
  fstream fin;
  fin.open(filename.c_str(), ios_base::in);
  if (!fin) 
    throw PLGLoaderException("file open failed");

  char cname[256] = {0};
  int num_verts = 0;
  int num_polys = 0;

  string linestr;
  while (std::getline(fin, linestr)) {
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
    if (!linestr.empty()) {
      if (*linestr.begin() == '#')
        continue;

      int v1,v2,v3;

      sscanf_s(linestr.c_str(), "%d %d %d", &v1, &v2, &v3);
      vlist.push_back({(double)v1 * scale, (double)v2* scale, (double)v3* scale});
      if (++vertsRead == num_verts)
        break;
    }
    linestr.clear();
  }

  if (vertsRead != num_verts)
    throw PLGLoaderException("vertsRead != num_verts");

  //poly list
  int polysRead = 0;
  linestr.clear();
  while (std::getline(fin, linestr)) {
    if (!linestr.empty()) {
      if (*linestr.begin() == '#')
        continue;

      unsigned int v1, v2, v3;
      int desc = 0;
      unsigned int num = 0;
      char strDesc[64] = {0};
      sscanf_s(linestr.c_str(), "%s %d %d %d %d", strDesc, 64, &num, &v1, &v2, &v3);

      if (strDesc[0] == '0' && toupper(strDesc[1]) == 'X')
        sscanf_s(strDesc, "%x", &desc);
      else
        desc = atoi(strDesc);

      Polygon<3> poly = {v1, v2, v3};


      const int PLX_COLOR_MODE_RGB_FLAG = 0x8000;
      const int PLX_2SIDED_FLAG = 0x1000;
      if (desc & PLX_2SIDED_FLAG)
        poly.setAttr(kPolygonAttr2Side);

      if (desc & PLX_COLOR_MODE_RGB_FLAG) {
        uint8_t red = ((desc & 0x0f00) >> 8) + 32;
        uint8_t green = ((desc & 0x00f0) >> 4) + 32;;
        uint8_t blue = (desc & 0x000f) + 32;;
        poly.setColor(Color(red, green, blue));
      } else {
        poly.setColor(Color(0xff, 0, 0));
      }

      #define PLX_SHADE_MODE_MASK   0x6000   // mask to extract shading mode
      // shading mode of polygon
      #define PLX_SHADE_MODE_PURE_FLAG      0x0000  // this poly is a constant color
      #define PLX_SHADE_MODE_CONSTANT_FLAG  0x0000  // alias
      #define PLX_SHADE_MODE_FLAT_FLAG      0x2000  // this poly uses flat shading
      #define PLX_SHADE_MODE_GOURAUD_FLAG   0x4000  // this poly used gouraud shading
      #define PLX_SHADE_MODE_PHONG_FLAG     0x6000  // this poly uses phong shading
      #define PLX_SHADE_MODE_FASTPHONG_FLAG 0x6000  // this poly uses phong shading (alias)

     int shadeMode = (desc & PLX_SHADE_MODE_MASK);
     // set polygon shading mode
     switch (shadeMode) {
          case PLX_SHADE_MODE_PURE_FLAG: {
            poly.setAttr(kPolygonAttrShadeModePureFlag);
          } break;

          case PLX_SHADE_MODE_FLAT_FLAG: {
            poly.setAttr(kPolygonAttrShadeModeFlatFlag);
          } break;

          case PLX_SHADE_MODE_GOURAUD_FLAG: {
         
          // the vertices from this polygon all need normals, set that in the flags attribute

          } break;

          case PLX_SHADE_MODE_PHONG_FLAG: {
          // the vertices from this polygon all need normals, set that in the flags attribute
          } break;

          default: break;
     } // end switch

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