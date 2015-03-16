#include "../Object.h"
#include "../Camera.h"
#include "../math/Math.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace s3d;


BOOST_AUTO_TEST_CASE(CameraUVN_unittest) {
  //const string s = "keep out";
  //auto &xss = s[0];
  //const string& sx = s;
  //auto &xxx = sx;

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

  {
    CameraUVN camera({0, 0, 0}, {0, 0, 1}, 90, 10, 1000, 100, 100);
    Point4FD pt[3] = {{0, 10, 10}, {10, 0, 10}, {-10, 0, 10}};
    auto n = Vector4FD(pt[0], pt[1]).crossProduct(Vector4FD(pt[1], pt[2]));
    BOOST_CHECK(n.z_ < 0.);
    for (auto p : pt)  {
      BOOST_CHECK(!camera.isBackface(p, n));
      BOOST_CHECK(camera.isBackface(p, n*-1));
    }
   }

  {
    CameraUVN camera({0, 0, 0}, {0, 0, 1}, 90, 10, 1000, 100, 100);
    Point4FD pt[3] = {{0, 10, 10}, {10, 0, 10}, {-10, 0, 10}};

    const auto n = Vector4FD(pt[0], pt[1]).crossProduct(Vector4FD(pt[1], pt[2]));
    BOOST_CHECK(n.z_ < 0.);
    for (int i = 0; i< 360; i += 10) {
      auto rmat = buildRotateMatrix4x4YXZ<double>(0, 0, degreeToRadius((double)i));
      for (auto &p : pt) {
        p = p * rmat;
      }

      for (auto p : pt) {
        BOOST_CHECK(!camera.isBackface(p, n));
        BOOST_CHECK(camera.isBackface(p, n*-1));
      }
    }
  }

  auto testBackfaceFun = [](double ay, double ax, double az, bool result) {
                  CameraUVN camera({0, 0, -300}, {0, 0, 1}, 90, 10, 1000, 100, 100);
                  Point4FD pt[3] = {{0, 10, 0}, {10, 0, 0}, {-10, 0, 0}};
                  {
                    auto u = Vector4FD(pt[0], pt[1]);
                    auto v = Vector4FD(pt[1], pt[2]);
                    auto n = u.crossProduct(v);
                    BOOST_CHECK(n.z_ < 0.);

                    for (auto p : pt) {
                      BOOST_CHECK(!camera.isBackface(p, n));
                      BOOST_CHECK(camera.isBackface(p, n*-1));
                    }
                  }

                  auto rmat = buildRotateMatrix4x4YXZ<double>(degreeToRadius(ay), degreeToRadius(ax), degreeToRadius(az));
                  for (auto &p : pt) {
                    p = p * rmat;
                  }

                  auto u = Vector4FD(pt[0], pt[1]);
                  auto v = Vector4FD(pt[1], pt[2]);
                  auto n = u.crossProduct(v);

                  for (auto p : pt) {
                    BOOST_CHECK(result == camera.isBackface(p, n));
                    BOOST_CHECK(!result == camera.isBackface(p, n*-1));
                  }
               };

  {
    testBackfaceFun(0, 89.0, 0, false);
    testBackfaceFun(0, 91.0, 0, true);

    testBackfaceFun(89.0, 0, 0, false);
    testBackfaceFun(91.0, 0, 0, true);
  }
}

