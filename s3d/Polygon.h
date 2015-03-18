#pragma once
#include <stdint.h>

#include "math/Point.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "Color.h"

namespace s3d
{

enum PolygonState {
  kPolygonStateInVisible,
  kPolygonStateVisible,
  kPolygonStateClipped,
  kPolygonStateBackface
};

enum PolygonAttr {
  kPolygonAttrVoid = 0,
  kPolygonAttr2Side,
  kPolygonAttrShadeModePureFlag,
  kPolygonAttrShadeModeFlatFlag,
  kPolygonAttrShadeModeGOURAUDFlag,
  kPolygonAttrShadeModePHONGFlag
};

template<size_t VertexNum>
class Polygon {
public:
  typedef uint32_t value_type;
  typedef uint32_t* iterator;
  typedef const uint32_t* const_iterator;

  Polygon() : state_(kPolygonStateInVisible), attr_(kPolygonAttrVoid) {
  }

  Polygon(const std::initializer_list<value_type>& ilist) : state_(kPolygonStateInVisible), attr_(kPolygonAttrVoid) {
    assert(ilist.size() == VertexNum);

    std::copy(ilist.begin(), ilist.end(), vertices);
  }

  Polygon(const Polygon& p) {
    color_ = p.color_;
    state_ = p.state_;
    attr_ = p.attr_;
    normal_ = p.normal_;
    std::copy(p.vertices, p.vertices + VertexNum, vertices);
  }

  Polygon& operator= (const Polygon& p) {
    color_ = p.color_;
    state_ = p.state_;
    attr_ = p.attr_;
    normal_ = p.normal_;
    std::copy(p.vertices, p.vertices + VertexNum, vertices);
    return *this;
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

  PolygonState getState() const {
    return state_;
  }

  void setState(PolygonState s) {
    state_ = PolygonState((unsigned)state_ | s);
  }

  PolygonAttr getAttr() const {
    return attr_;
  }

  void setAttr(PolygonAttr s) {
    attr_ = PolygonAttr((unsigned)attr_ | s);
  }

  //template<typename VertexList, typename MATRIX>
  //void transform(VertexList& vl, const MATRIX& mat) {
  //  for (auto& ptIndex : *this) {
  //    vl[ptIndex] = vl[ptIndex] * mat;
  //  }
  //}

  template <typename VertexList>
  void computeNormal(VertexList& vl, bool isNormalize = true) {
  //FIXEME
    Vector4FD u((Point3F)vl[vertices[0]], (Point3F)vl[vertices[1]]);
    Vector4FD v((Point3F)vl[vertices[1]], (Point3F)vl[vertices[2]]);

    normal_ = u.crossProduct(v);

    if (isNormalize)
      normal_.normalizeSelf();
  }

  
  Vector4FD getNormal() const {
    return normal_;
  }

private:
  Vector4FD normal_;

  PolygonState state_;
  PolygonAttr attr_;

  value_type vertices[VertexNum];
  Color color_;

  int id;
};

typedef Polygon<3U> Polygon3;

}// s3d
