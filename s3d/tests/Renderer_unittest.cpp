#include "../Object.h"
#include "../Camera.h"
#include "../math/Math.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>
#include "../Renderer.h"

using namespace std;
using namespace s3d;


BOOST_AUTO_TEST_CASE(RendererDrawLine2D_Horizontal_unittest) {
  {
    uint32_t  buffer[32][32] = {0};
    Renderer r(&buffer[0][0], 32, 32);

    Color c(255, 0, 0), c2(0, 255, 0);
    BOOST_CHECK(c == c);
    BOOST_CHECK(c != c2);

    r.drawPixel2D({0, 0}, c);
    BOOST_CHECK(Color(buffer[0][0]) == c);

    r.drawPixel2D({31, 31}, c);
    BOOST_CHECK(Color(buffer[31][31]) == c);

    Point2<float> pt1(0, 23);
    Point2<float> pt2(31, 23);
    r.drawLine2D_Horizontal(pt1, pt2, c);

    for (int i = 0; i<31; i++) {
      BOOST_REQUIRE_MESSAGE(Color(buffer[23][i]) == c, "i=" << i);
    }

    BOOST_REQUIRE(Color(buffer[23][31]) != c);

    {
      Point2<float> pt1(11, 24);
      Point2<float> pt2(21, 24);
      r.drawLine2D_Horizontal(pt1, pt2, c);
      for (int i = 11; i<21; i++) {
        BOOST_REQUIRE_MESSAGE(Color(buffer[24][i]) == c, "i=" << i);
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(RendererFillTirangle_unittest) {
  {
    uint32_t  buffer[32][32] = {0};
    Renderer r(&buffer[0][0], 32, 32);;

    Color c(255, 0, 0), c2(0, 255, 0);
    Point2<float> pt1(13, 0);
    Point2<float> pt2(1, 24);
    Point2<float> pt3(21, 24);

    r.fillFlatBottomTriangle2D(pt1, pt2, pt3, c);

    BOOST_REQUIRE(Color(buffer[0][13]) == 0);
    BOOST_REQUIRE(Color(buffer[24][21]) == 0);

   
   /* const auto height = pt2.y_ - pt1.y_;
    const auto m_left = (pt2.x_ - pt1.x_) / height;
    const auto m_right = (pt3.x_ - pt1.x_) / height;

    const auto ye = pt1.y_ + height;*/

  }

  {
    uint32_t  buffer[1032][1032] = {0};
    Renderer r(&buffer[0][0], 1032, 1032);;

    Color c(255, 0, 0), c2(0, 255, 0);
    Point2<double> pt1(816.62335205078125, 283.43658447265625);
    Point2<double> pt2(852.07098388671875 , 248.01094055175781);
    Point2<double> pt3(587.92901611328125 , 248.01094055175781);

    r.fillFlatTopTriangle2D(pt1, pt2, pt3, c);

    BOOST_REQUIRE(Color(buffer[0][13]) == 0);
    BOOST_REQUIRE(Color(buffer[24][21]) == 0);


    /* const auto height = pt2.y_ - pt1.y_;
    const auto m_left = (pt2.x_ - pt1.x_) / height;
    const auto m_right = (pt3.x_ - pt1.x_) / height;

    const auto ye = pt1.y_ + height;*/


  }
}