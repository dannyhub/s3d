#pragma once
#include <stdint.h>

#include "math/Point.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "Color.h"

namespace s3d
{
template<size_t VertexNum>
class Polygon {
public:
  typedef uint32_t value_type;
  typedef uint32_t* iterator;
  typedef const uint32_t* const_iterator;

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

  value_type operator[] (size_t index) const {
    assert (index < VertexNum);
    return vertices[index];
  }

  value_type at(size_t index) const  {
    if (index >= VertexNum) {
      throw std::out_of_range("Polygon at out of range");
    }

    return vertices[index];
  }

  Vector4FD normal_;

private:
  value_type vertices[VertexNum];
  Color color_;

  Vector4FD n_;

  int id;
  int state;
  int attr;
};

}// s3d
