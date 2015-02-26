#pragma once
#include "math/Point.h"
#include "math/Vector.h"
#include "math/Matrix.h"

#include <vector>

namespace s3d
{
  
template<typename T>
class VertexList {
public:
  typedef T value_type;
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::size_type size_type;

  VertexList() {
  }

  VertexList(const std::initializer_list<value_type>& ilist) {
    std::copy(ilist.begin(), ilist.end(), std::back_insert(vertices));
  }

  void push_back(const T& pt) {
    vertices.push_back(pt);
  }

  void clear() {
    vertices.clear();
  }

  T operator[] (size_type index) {
    assert(index < vertices.size());
    return vertices[index];
  }

  T at (size_type index) {
    assert(index < vertices.size());
    return vertices.at(index);
  }

  iterator begin() {
    return vertices.begin();
  }

  iterator end() {
    return vertices.end();
  }

  const_iterator begin() const {
    return vertices.begin();
  }

  const_iterator end() const {
    return vertices.end();
  }

  size_type size() const {
    return vertices.size();
  }

private:
  std::vector<T> vertices;
};


}// s3d