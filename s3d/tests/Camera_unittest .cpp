#include "../Object.h"
#include "../Camera.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace s3d;


BOOST_AUTO_TEST_CASE(CameraUVN_unittest) {
  const string s = "keep out";
  auto &xss = s[0];
  const string& sx = s;
  auto &xxx = sx;

  for (const auto &c  : s){
  }

  {
    CameraUVN camera({0, 0, -100}, {0, 0, 1}, 90, 10, 1000, 100, 100);
    Point4FD pt{1, 1, 1};
    pt = pt * camera.getWorldToCameraMatrix4x4FD();
    BOOST_CHECK_EQUAL(pt.x_, 1);
    BOOST_CHECK_EQUAL(pt.y_, 1);
    BOOST_CHECK_EQUAL(pt.z_, 101);
    BOOST_CHECK_EQUAL(pt.w_, 1);
  }

  {
    CameraUVN camera({1, 2, 3}, {1, 2, 4}, 40, 10, 1000, 100, 100);
    Point4FD pt{3, 4, 5};
    pt *=  camera.getWorldToCameraMatrix4x4FD();
    BOOST_CHECK_EQUAL(pt.x_, 2);
    BOOST_CHECK_EQUAL(pt.y_, 2);
    BOOST_CHECK_EQUAL(pt.z_, 2);
    BOOST_CHECK_EQUAL(pt.w_, 1);
  }

  {

    CameraUVN camera({0, 0, 0}, {4, 4, 4}, 40, 10, 1000, 100, 100);
    Point4FD pt{1, 1, 1};
    Point4FD ptTrans = pt * camera.getWorldToCameraMatrix4x4FD();

    BOOST_CHECK_EQUAL(pt.dotProduct(camera.getU()), ptTrans.x_);
    BOOST_CHECK_EQUAL(pt.dotProduct(camera.getV()), ptTrans.y_);
    BOOST_CHECK_EQUAL(pt.dotProduct(camera.getN()), ptTrans.z_);
    BOOST_CHECK_EQUAL(ptTrans.w_, 1);
  }

}