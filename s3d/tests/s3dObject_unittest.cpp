#include "../Object.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>

using namespace s3d;


BOOST_AUTO_TEST_CASE(s3dObject_unittest) {

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

  Object obj(1, "testobj");
  obj.addVertex({0,4,3});
  obj.addVertex({4, -4, 3});
  obj.addVertex({-4, -4, 3});

  obj.addPolygon({0, 1, 2});

  double wx = 100;
  double wy = 200;
  double wz = 1300;
  addToWorld(obj, wx, wy, wz);
  {
    auto it = obj.localVertexList_.begin();
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      const auto pt = *it;
      const auto transpt = *transVerit;
      BOOST_CHECK_EQUAL(transpt.x_, pt.x_ + 100);
      BOOST_CHECK_EQUAL(transpt.y_, pt.y_ + 200);
      BOOST_CHECK_EQUAL(transpt.z_, pt.z_ + 1300);
      BOOST_CHECK_EQUAL(transpt.w_, 1);

      ++it;
      ++transVerit;
    }
  }

  double anglex = 0;
  double angley = 0;
  double anglez = 0;

  setCamera(obj, {100, 200, 100}, anglex, angley, anglez);
  {
    //rotate with: YXZ
    anglex = -anglex;
    angley = -angley;
    anglez = -anglez;

    double x, y, z;

    auto it = obj.localVertexList_.begin();
    auto transVerit = obj.transVertexList_.begin();
    while (it != obj.localVertexList_.end()) {
      auto pt = *it;
      const auto transpt = *transVerit;

      //ToWorld
      pt.x_ += wx;
      pt.y_ += wy;
      pt.z_ += wz;

      //to camera
      pt.x_ -= 100;
      pt.y_ -= 200;
      pt.z_ -= 100;

      //y
      x = pt.x_ * cos(angley) - pt.z_ * sin(angley);
      z = pt.x_ * sin(angley) + pt.z_ * cos(angley);
      pt.x_ = x;
      pt.z_ = z;

      //x
      y = pt.y_ * cos(anglex) - pt.z_ * sin(anglex);
      z = pt.y_ * sin(anglex) + pt.z_ * cos(anglex);
      pt.y_ = y;
      pt.z_ = z;

      //z
      x = pt.x_ * cos(anglez) - pt.y_ * sin(anglez);
      y = pt.x_ * sin(anglez) + pt.y_ * cos(anglez);
      pt.x_ = x;
      pt.y_ = y;

      //BOOST_CHECK_EQUAL(transpt.x_, pt.x_);
      //BOOST_CHECK_EQUAL(transpt.y_, pt.y_);
      //BOOST_CHECK_EQUAL(transpt.z_, pt.z_);
      //BOOST_CHECK_EQUAL(transpt.w_, 1);

      BOOST_CHECK(vector_impl::equalZero(transpt.x_ - pt.x_));
      BOOST_CHECK(vector_impl::equalZero(transpt.y_ - pt.y_));
      BOOST_CHECK(vector_impl::equalZero(transpt.z_ - pt.z_));
      BOOST_CHECK(vector_impl::equalZero(transpt.w_ - 1));

      ++it;
      ++transVerit;
    }
  }

  {
    int viewWidth = 600;
    int viewHeight = 600;

    auto vervices = obj.transVertexList_;
    perspectiveProject(obj, viewWidth, viewHeight);

    auto it = vervices.begin();
    auto transVerit = obj.transVertexList_.begin();

    //nearsZ = 1
    //widthHeightRate = viewWidth / viewHeight
    //px = x / z, [-1,1]
    //py = y / z, [-1,1]

    //vx = (px + 1)  * (viewWidth-1) / 2 = (px + 1)  * ((viewWidth/2) - 1/2)
    //vy = (viewHeight - 1) - (py + 1)  * (viewHeight-1) / 2 = (viewHeight - 1) - (py + 1)  * ((viewHeight/2) - 1/2)
    while (it != vervices.end()) {
      auto pt = *it;
      const double px = (double)pt.x_ / pt.z_;// [-1,1]
      const double py = (double)pt.y_ / pt.z_;// [-1, 1]

      const auto xr = (double)viewWidth / 2. - 0.5;
      const auto yr = (double)viewHeight / 2. - 0.5;

      const double vx = px * xr + xr;
      const double vy = double(viewHeight - 1) - py * yr - yr;
      BOOST_CHECK(vx >= 0 && vx < viewWidth);
      BOOST_CHECK(vy >= 0 && vy < viewHeight);

      BOOST_CHECK_EQUAL(transVerit->y_, vy);
      BOOST_CHECK(vector_impl::equalZero(transVerit->x_ - vx));
      //BOOST_CHECK(vector_impl::equalZero(transVerit->y_ - vy));
      ++it;
      ++transVerit;
    }

  }


}