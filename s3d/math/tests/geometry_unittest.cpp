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

  Point3F pt3(1, 2, 3);
  CylindricalPointf cpt(pt3);
  BOOST_CHECK(cpt.toPoint() == pt3);

  vector<string> V(10,"H");
  vector<string> v2(10);

}