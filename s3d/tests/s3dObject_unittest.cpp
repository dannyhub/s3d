#include "../Object.h"
#include "../math/Math.h"
#include "../math/Matrix.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>

using namespace s3d;
using namespace std;

BOOST_AUTO_TEST_CASE(s3dObject_unittest1) {
  auto pt  = Vertex::PointType{1, 2, 3};
  Vertex vertex(pt);
  BOOST_CHECK(pt == vertex.getPoint());

  auto pt2 = Vertex::PointType{11, 22, 33};
  vertex.setPoint(pt2);
  BOOST_CHECK(pt2 == vertex.getPoint());

  Matrix<float, 4U, 4U> mat {3,0,0,0,
                             0,3,0,0,
                             0,0,3,0,
                             0,0,0,1};

  Vertex vertex3(pt);
  Vertex vertex4 = vertex3 * mat;
  BOOST_CHECK(vertex4.getPoint() == pt * 3);

  vertex3 = vertex3 * mat;
  BOOST_CHECK(vertex3.getPoint() == pt * 3);

  Polygon3 poly = {0,1,2};
  Polygon3::value_type i = 0;
  for (auto v : poly) {
    BOOST_CHECK(v == i++);
  }

  Polygon3 poly2{2, 1, 0};
  i = 2;
  for (auto v : poly2) {
    BOOST_CHECK(i < 3 && i >=0);
    BOOST_CHECK(v == i--);
  }

  BOOST_CHECK(poly2.size() == 3);
  BOOST_CHECK(poly2[0] == 2);
  BOOST_CHECK(poly2[1] == 1);
  BOOST_CHECK(poly2[2] == 0);
  BOOST_CHECK(poly2.at(0) == 2);
  BOOST_CHECK(poly2.at(1) == 1);
  BOOST_CHECK(poly2.at(2) == 0);

  BOOST_CHECK_THROW(poly2.at(3), std::out_of_range);
  BOOST_CHECK_THROW(poly2.at((size_t)-1), std::out_of_range);
  BOOST_CHECK_THROW(poly2.at(4), std::out_of_range);

  Polygon3 poly3(poly2);
  i = 2;
  for (auto v : poly3) {
    BOOST_CHECK(i < 3 && i >= 0);
    BOOST_CHECK(v == i--);
  }

  Polygon3 poly4;
  poly4 = poly3;
  i = 2;
  for (auto v : poly4) {
    BOOST_CHECK(i < 3 && i >= 0);
    BOOST_CHECK(v == i--);
  }
  BOOST_CHECK(poly4.size() == 3);
  BOOST_CHECK(poly4[0] == 2);
  BOOST_CHECK(poly4[1] == 1);
  BOOST_CHECK(poly4[2] == 0);
  BOOST_CHECK(poly4.at(0) == 2);
  BOOST_CHECK(poly4.at(1) == 1);
  BOOST_CHECK(poly4.at(2) == 0);

  BOOST_CHECK_THROW(poly4.at(3), std::out_of_range);
  BOOST_CHECK_THROW(poly4.at((size_t)-1), std::out_of_range);
  BOOST_CHECK_THROW(poly4.at(4), std::out_of_range);

  Object obj;
  Object::VertexType v1[3] = {pt, pt * 2, pt * 3};
  
  Object::PolygonType polys[3] = {poly, poly2, poly2};
  auto sdfsdf = polys[0];
  //obj.addPolygon({polys[0], polys[0], polys[0]});
  obj.addPolygon(polys[0]);
  obj.addPolygon(polys[1]);
  obj.addPolygon(polys[2]);

  BOOST_CHECK(obj.size() == 3);
  i = 0;
  
  for (auto &poly : obj) {
    int j = 0;
    for (auto v : poly) {
      auto p = polys[i];
      BOOST_CHECK_EQUAL(p[j++], v);
    }
    ++i;
  }

  VertexList<Point2FD> vl{Point2FD(1, 2), Point2FD(3, 4)};
  BOOST_CHECK(vl.size() == 2);
  vl.push_back(Point2FD(5, 6));

  BOOST_CHECK(vl.size() == 3);

}

BOOST_AUTO_TEST_CASE(s3dObject_unittest2) {

}

BOOST_AUTO_TEST_CASE(s3dObject_unittest3) {
  {
    Point4<double> pt4 = {1,2,3};
    Matrix4x4FD mat = {1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1 };
   
    auto res = pt4 * mat;
    BOOST_CHECK(res == pt4);

    mat *= 3;
    mat[3][3] = 1;
    auto res2 = pt4 * mat;
    BOOST_CHECK(res2 == pt4 * 3);

  }

    {
      Point4<double> pt4 = {1,2,3};
      Matrix4x4FD mat = {1, 2, 2, 0,
                         2, 2, -2, 0,
                         3, 3, 3, 0,
                         4, 4, 4, 1};
   
      auto res = pt4 * mat;
      pt4.x_ = 18;
      pt4.y_ = 19;
      pt4.z_ = 11;
      BOOST_CHECK(res == pt4);

      Matrix1x4FD mat1x4 = {1,2,3,1};
      auto res3 = mat1x4 * mat;
      BOOST_CHECK_EQUAL(res3.at(0,0), 18);
      BOOST_CHECK_EQUAL(res3.at(0, 1), 19);
      BOOST_CHECK_EQUAL(res3.at(0, 2), 11);
      BOOST_CHECK_EQUAL(res3.at(0, 3), 1);
  }

  //Object obj(1, "testobj");
  //obj.addVertex({0,4,3});
  //obj.addVertex({4, -4, 3});
  //obj.addVertex({-4, -4, 3});

  //obj.addPolygon({0, 1, 2});

  //double wx = 100;
  //double wy = 200;
  //double wz = 1300;
  //addToWorld(obj, wx, wy, wz);
  //{
  //  auto it = obj.localVertexList_.begin();
  //  auto transVerit = obj.transVertexList_.begin();
  //  while (transVerit != obj.transVertexList_.end()) {
  //    const auto pt = *it;
  //    const auto transpt = *transVerit;
  //    BOOST_CHECK_EQUAL(transpt.x_, pt.x_ + 100);
  //    BOOST_CHECK_EQUAL(transpt.y_, pt.y_ + 200);
  //    BOOST_CHECK_EQUAL(transpt.z_, pt.z_ + 1300);
  //    BOOST_CHECK_EQUAL(transpt.w_, 1);

  //    ++it;
  //    ++transVerit;
  //  }
  //}

  //double anglex = 0;
  //double angley = 0;
  //double anglez = 0;

  //setCamera(obj, {100, 200, 100}, anglex, angley, anglez);
  //{
  //  //rotate with: YXZ
  //  anglex = -anglex;
  //  angley = -angley;
  //  anglez = -anglez;

  //  double x, y, z;

  //  auto it = obj.localVertexList_.begin();
  //  auto transVerit = obj.transVertexList_.begin();
  //  while (it != obj.localVertexList_.end()) {
  //    auto pt = *it;
  //    const auto transpt = *transVerit;

  //    //ToWorld
  //    pt.x_ += wx;
  //    pt.y_ += wy;
  //    pt.z_ += wz;

  //    //to camera
  //    pt.x_ -= 100;
  //    pt.y_ -= 200;
  //    pt.z_ -= 100;

  //    //y
  //    x = pt.x_ * cos(angley) - pt.z_ * sin(angley);
  //    z = pt.x_ * sin(angley) + pt.z_ * cos(angley);
  //    pt.x_ = x;
  //    pt.z_ = z;

  //    //x
  //    y = pt.y_ * cos(anglex) - pt.z_ * sin(anglex);
  //    z = pt.y_ * sin(anglex) + pt.z_ * cos(anglex);
  //    pt.y_ = y;
  //    pt.z_ = z;

  //    //z
  //    x = pt.x_ * cos(anglez) - pt.y_ * sin(anglez);
  //    y = pt.x_ * sin(anglez) + pt.y_ * cos(anglez);
  //    pt.x_ = x;
  //    pt.y_ = y;

  //    BOOST_CHECK(vector_impl::equalZero(transpt.x_ - pt.x_));
  //    BOOST_CHECK(vector_impl::equalZero(transpt.y_ - pt.y_));
  //    BOOST_CHECK(vector_impl::equalZero(transpt.z_ - pt.z_));
  //    BOOST_CHECK(vector_impl::equalZero(transpt.w_ - 1));

  //    ++it;
  //    ++transVerit;
  //  }
  //}

  //{
  //  int viewWidth = 600;
  //  int viewHeight = 600;

  //  auto vervices = obj.transVertexList_;
  //  perspectiveProject(obj, viewWidth, viewHeight);

  //  auto it = vervices.begin();
  //  auto transVerit = obj.transVertexList_.begin();

  //  //nearsZ = 1
  //  //widthHeightRate = viewWidth / viewHeight
  //  //px = x / z, [-1,1]
  //  //py = y / z, [-1,1]

  //  //vx = (px + 1)  * (viewWidth-1) / 2 = (px + 1)  * ((viewWidth/2) - 1/2)
  //  //vy = (viewHeight - 1) - (py + 1)  * (viewHeight-1) / 2 = (viewHeight - 1) - (py + 1)  * ((viewHeight/2) - 1/2)
  //  while (it != vervices.end()) {
  //    auto pt = *it;
  //    const double px = (double)pt.x_ / pt.z_;// [-1,1]
  //    const double py = (double)pt.y_ / pt.z_;// [-1, 1]

  //    const auto xr = (double)viewWidth / 2. - 0.5;
  //    const auto yr = (double)viewHeight / 2. - 0.5;

  //    const double vx = px * xr + xr;
  //    const double vy = double(viewHeight - 1) - py * yr - yr;
  //    BOOST_CHECK(vx >= 0 && vx < viewWidth);
  //    BOOST_CHECK(vy >= 0 && vy < viewHeight);

  //    BOOST_CHECK_EQUAL(transVerit->y_, vy);
  //    BOOST_CHECK(vector_impl::equalZero(transVerit->x_ - vx));
  //    //BOOST_CHECK(vector_impl::equalZero(transVerit->y_ - vy));
  //    ++it;
  //    ++transVerit;
  //  }

  //}


}