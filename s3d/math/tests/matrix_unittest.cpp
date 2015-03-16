#include "../Matrix.h"
#include "../Math.h"

#define BOOST_TEST_MODULE unittest
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>

using namespace s3d;

BOOST_AUTO_TEST_CASE(testMatrix2x2) {
 
  {
    Matrix2x2F m1 = {1, 2, 3, 4};
    Matrix2x2F m2 = {11, 12, 13, 14};
    Matrix2x2F m3 = {12, 14, 16, 18};

    Matrix2x2F m4 = {10, 20, 30, 40};
    Matrix2x2F m5 = {7, 10, 15, 22};
    {
      BOOST_CHECK(m1 == m1);
      BOOST_CHECK(m2 == m2);

      Matrix2x2F m = m1 + m2;

      BOOST_CHECK(m1 != m2);
      BOOST_CHECK(m == m3);

      BOOST_CHECK(m3 - m2 == m1);
      BOOST_CHECK(m3 - m1 == m2);

      m -= m1;
      BOOST_CHECK(m == m2);

      m += m1;
      BOOST_CHECK(m == m3);

      m = m1 * 10.f;
      BOOST_CHECK(m == m4);

      m = m1 * m1;
      BOOST_CHECK(m == m5);
    }


    BOOST_CHECK_EQUAL(matrixDet(m1), -2);
    BOOST_CHECK_EQUAL(matrixDet(m1), matrix_impl::computeDet(m1));

    BOOST_CHECK_EQUAL(matrixDet(m5), matrix_impl::computeDet(m5));

    {
      auto m  = matrixInverse(m1);
      auto resm = m1 * m;
      BOOST_CHECK(resm.isIdentify());
    }

    {
      auto m = matrixInverse(m3);
      auto resm = m3 * m;
      BOOST_CHECK(resm.isIdentify());
    }

    {
      Matrix2x2F m1 = {1, 2, 1, 2};
      auto a = m1.det();
      BOOST_CHECK(a == 0.);
      BOOST_CHECK_THROW(m1.inverse(), MatrixInverseException);
    }
  }
}

BOOST_AUTO_TEST_CASE(testMatrix3x3) {
  {
    const Matrix3x3F m1 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Matrix3x3F m2 = {11, 12, 13, 14, 15, 16, 17, 18, 19};
    Matrix3x3F m3 = {12, 14, 16, 18, 20, 22, 24, 26, 28};
    Matrix3x3F m4 = {2, 0, 0, 0, 2, 0, 0, 0, 2};
    Matrix3x3F m5 = {2, 11, 0, 0, 2, 33, 44, 0, 2};
    {
      BOOST_CHECK(m1 == m1);
      BOOST_CHECK(m2 == m2);

      Matrix3x3F m = m1 + m2;

      BOOST_CHECK(m1 != m2);
      BOOST_CHECK(m == m3);

      BOOST_CHECK(m3 - m2 == m1);
      BOOST_CHECK(m3 - m1 == m2);

      m -= m1;
      BOOST_CHECK(m == m2);

      m += m1;
      BOOST_CHECK(m == m3);
    }


    {
      auto m = m1 * 12;
      BOOST_CHECK(m == m1 * 12);
    }

    {
      auto m = m1;
      m *= 12;

      BOOST_CHECK(m == m1 * 12);
    }

    BOOST_CHECK_EQUAL(matrixDet(m1), matrix_impl::computeDet(m1));
    BOOST_CHECK_EQUAL(m1.det(), matrix_impl::computeDet(m1));

    BOOST_CHECK_EQUAL(matrixDet(m4), 8);
    BOOST_CHECK_EQUAL(matrixDet(m4), matrix_impl::computeDet(m4));
    BOOST_CHECK_EQUAL(matrixDet(m3), matrix_impl::computeDet(m3));



    {
      auto m = matrixInverse(m4);
      auto resm = m4 * m;
      BOOST_CHECK(resm.isIdentify());
    }

    {
      auto m = m4.inverse();
      auto resm = m4 * m;
      BOOST_CHECK(resm.isIdentify());
    }

    {
      auto m = matrixInverse(m5);
      auto resm = m5 * m;

      auto resm2 = resm * m5;
      BOOST_CHECK(resm.isIdentify());
    }

    {
      Matrix3x3F m1 = {1, 2, 3, 1, 2, 3, 1, 2, 3};
      auto a = m1.det();
      BOOST_CHECK(a == 0.);
      BOOST_CHECK_THROW(m1.inverse(), MatrixInverseException);
    }
  }
}


BOOST_AUTO_TEST_CASE(testMatrix4x4) {
  {
    const Matrix4x4F m1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 11, 12, 13};
    {
      BOOST_CHECK_EQUAL(matrixDet(m1), matrix_impl::computeDet(m1));
      BOOST_CHECK_EQUAL(m1.det(), matrix_impl::computeDet(m1));
    }
  }

  {
    auto rmat = buildRotateMatrix4x4YXZ<double>(0.5, 0.2, 0.3);
    auto pt = buildIdentifyMatrix4x4<double>();

    auto s = pt * rmat;
    BOOST_CHECK(s == rmat);
  }

  {//rotate z
    auto rmat = buildRotateMatrix4x4YXZ<double>(0, 0, degreeToRadius(90.));
    Matrix<double,1,4> pt = {1,0,0,1};
    Matrix<double, 1, 4> s = pt * rmat;

    auto d = degreeToRadius(90.);
    double tx = ::cos(d) - 0 * sin(d);
    double ty = ::sin(d) + 0 * cos(d);

    BOOST_CHECK(s[0][0] == tx);
    BOOST_CHECK(equalZero(tx));

    BOOST_CHECK(s[0][1] == ty);
    BOOST_CHECK(equalZero(ty - 1.));
  }

  {//rotate y
    auto rmat = buildRotateMatrix4x4YXZ<double>(degreeToRadius(90.), 0, 0);
    Matrix<double, 1, 4> pt = {0, 0, 1, 1};
    Matrix<double, 1, 4> s = pt * rmat;

    auto d = degreeToRadius(90.);
    double tz = 1 * ::cos(d) - 0 * sin(d);
    double tx = 1 * ::sin(d) + 0 * cos(d);

    BOOST_CHECK(s[0][0] == tx);
    BOOST_CHECK(equalZero(tx - 1.));

    BOOST_CHECK(s[0][2] == tz);
    BOOST_CHECK(equalZero(tz ));
  }

  {//rotate x
    auto rmat = buildRotateMatrix4x4YXZ<double>(0, degreeToRadius(90.), 0);
    Matrix<double, 1, 4> pt = {0, 1, 0, 1};
    Matrix<double, 1, 4> s = pt * rmat;

    auto d = degreeToRadius(90.);
    double ty = 1 * ::cos(d) - 0 * sin(d);
    double tz = 1 * ::sin(d) + 0 * cos(d);

    BOOST_CHECK(s[0][1] == ty);
    BOOST_CHECK(equalZero(ty));

    BOOST_CHECK(s[0][2] == tz);
    BOOST_CHECK(equalZero(tz - 1.));
  }
}