#pragma once
#include "math/Point.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "Color.h"

namespace s3d
{
template<size_t VertexNum>
class Polygon {
public:
  typedef int value_type;
  typedef int* iterator;
  typedef const int* const_iterator;

  Polygon(const std::initializer_list<value_type>& ilist) {
    assert(ilist.size() == VertexNum);

    std::copy(ilist.begin(), ilist.end(), vertices);
  }

  Color getColor() const {
    return color_;
  }

  void setColor(const Color& c) {
    color_ = c;
  }
  
  iterator begin() {
    return vertices;
  }

  iterator end() {
    return vertices + VertexNum;
  }

  const_iterator begin() const {
    return vertices;
  }

  const_iterator end() const {
    return vertices + VertexNum;
  }

  size_t size() const {
    return VertexNum;
  }

private:
  int vertices[VertexNum];
  Color color_;

  int id;
  int state;
  int attr;
};

}// s3d
