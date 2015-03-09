#pragma once
#include "Color.h"
#include <string>

namespace s3d
{

class Materail {
public:
  Materail();
  ~Materail();

private:
  std::string name_;
  Color color_;

  float ka_, kd_, ks_; //ambient, diffuse, specular
  float ra_, rd_, rs_; //reflectivities ra_ = color_ * ka_
};

}// s3d

