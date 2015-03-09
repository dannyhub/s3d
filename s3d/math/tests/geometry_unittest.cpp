#include "../geometry.h"


#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace s3d;


BOOST_AUTO_TEST_CASE(geometry_unittest) {
  Point2F pt2(1,4);
  PolarPointf ppt(pt2);
  BOOST_CHECK(ppt.toPoint() == pt2);
  const int &xxx = 33;
  decltype(xxx) aa = 44;
  int a[10];
  auto ba = a;
  decltype(a) aaa;

  Point3F pt3(1, 2, 3);
  CylindricalPointf cpt(pt3);
  BOOST_CHECK(cpt.toPoint() == pt3);

  vector<string> V(10,"H");
  vector<string> v2(10);

  //float a = 3.f;
  //float b = a / 0.f;

  //float c = b + 10000;
  //BOOST_CHECK(b == c);
  //b = -b;
  //float at = ::atan(b);
  //float xx = ::tan(at);

  //float xx3 = ::atan2(3.f, 0.f);
  //c = 0.f;
  //std::numeric_limits<double> 
}