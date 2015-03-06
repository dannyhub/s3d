#include "../Vector.h"


#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace s3d;

struct tetsxx {
  string xx = "aaa";
  int a = 333;
  double b = 0.5;
//  vector<int> dsfddd{1,3,4};
};

void foo(int&) {
}

void foo(int) {
}

BOOST_AUTO_TEST_CASE(testVector2) {
  //tetsxx testxxxx = {string("bbb"), 333, 0.1};
  const int aadxd =33;
  const int &xsfa = aadxd;
  int xaaa;
  int &a = xaaa = 111;
  int &sdd = a;
  sdd = 1132;
  //foo(sdd);
  const int* pp =  &a;
  const int*  &sdff = pp;
  //int a[sdff] = {0};
  const char* xxss = typeid(sdff).name();

  Vector2F v1 = {1,2};
  Vector2F v2(3. , 4.);
  Vector2F v3(4., 6.);
  
  Vector2F v4 = {3, 6};
  BOOST_CHECK(v1 == v1);
  BOOST_CHECK(v2 == v2);
  BOOST_CHECK(v1 != v2);

  {
    auto v = v1 + v2;
    BOOST_CHECK(v == v3);
    v += v1;
    v -= v1;
    BOOST_CHECK(v == v3);
  }
  
  {
    auto v = v1 * 3;
    BOOST_CHECK(v == v4);
  }

  {
    auto v = 3 * v1;
    BOOST_CHECK(v == v4);

    BOOST_CHECK_EQUAL(v2.length(), 5);

  }

  {
    auto v = v3.normalize();
    BOOST_CHECK( ::fabs(v2.length() - 1.f) < 1E5 );
  }

  {
    auto v = v1.dotProduct(v2);
    BOOST_CHECK_EQUAL(v, 11);
  }

  {
    Vector2F v1 = {0, 200};
    Vector2F v2 = {200, 0};
    auto angle = v1.angle(v2) *  180.f / 3.1415927f;
    BOOST_CHECK(vector_impl::equalZero(angle - 90.f) == true);
    //printf("angle %f\n", angle);
  }

  {
    Vector2F v1 = {200, 200};
    Vector2F v2 = {200, 0};
    auto angle = v1.angle(v2) *  180.f / 3.1415927f;
    BOOST_CHECK_EQUAL(vector_impl::equalZero(angle - 45.f), true);
   // printf("angle %f\n", angle);

    //float dd = 0.2f;
   // void* xx = &dd;

    //printf("0.2 %X\n", *(int*)xx);
  }

  {
    Vector2F v1 = {200, 200};
    Vector2F v2 = {200, 0};
    auto v = v1.projectOnto(v2);
    //BOOST_CHECK_EQUAL(v, v1.length() * ::cos(45.f * 3.1415927f / 180.f));
  }

  const int i = 42;
  auto j= i;
  const auto &k = i;
  auto *p = &i;
  const auto j2 = i, &k2 = i;
}