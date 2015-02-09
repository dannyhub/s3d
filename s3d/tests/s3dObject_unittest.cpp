#include "../Object.h"

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>

using namespace s3d;

void addToWorld(Object& obj, float x, float y, float z) {
  Matrix4x4F translateMat = { 1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              x, y, z, 1 };

  obj.setWorldPosition({x, y, z});
  obj.transVertexList_.clear();
  auto it = obj.localVertexList_.begin();
  while (it != obj.localVertexList_.end()) {
    const auto pt4 = *it;
    obj.transVertexList_.push_back(pt4 * translateMat);
    ++it;
  }
}

void setCamera(Object& obj, Point4F pt, float anglex, float angley, float anglez) {
  Matrix4x4F translateMat = { 1.f, 0, 0, 0,
                              0, 1.f, 0, 0,
                              0, 0, 1.f, 0,
                              -pt.x_, -pt.y_, -pt.z_, 1.f};

  //rotate with: YXZ
  Matrix4x4F rotateYMat = {::cos(angley), 0, -::sin(angley), 0,
                            0, 1.f, 0, 0,
                            ::sin(angley), 0, ::cos(angley), 0,
                            0, 0, 0, 1.f };

  Matrix4x4F rotateXMat = { 1.f, 0, 0, 0,
                            0, ::cos(anglex), -::sin(anglex), 0,
                            0, ::sin(anglex), ::cos(anglex), 0,
                            0, 0, 0, 1.f };


  Matrix4x4F rotateZMat = {::cos(anglez), -::sin(anglez), 0, 0,
                            ::sin(anglez), ::cos(anglez), 0, 0,
                            0, 0, 1.f, 0,
                            0, 0, 0, 1.f };

  const auto mat = translateMat * rotateYMat  * rotateXMat ;
  auto transVerit = obj.transVertexList_.begin();
  while (transVerit != obj.transVertexList_.end()) {
    *transVerit = *transVerit * translateMat * rotateYMat  * rotateXMat * rotateZMat;
    ++transVerit;
  }
}

BOOST_AUTO_TEST_CASE(s3dObject_unittest) {

  {
    Point4<float> pt4 = {1,2,3};
    Matrix4x4F mat = { 1.f, 0, 0, 0,
                       0, 1.f, 0, 0,
                       0, 0, 1.f, 0,
                       0, 0, 0, 1.f };
   
    auto res = pt4 * mat;
    BOOST_CHECK(res == pt4);

    mat *= 3.f;
    mat[3][3] = 1.f;
    auto res2 = pt4 * mat;
    BOOST_CHECK(res2 == pt4 * 3.f);

  }

    {
      Point4<float> pt4 = {1,2,3};
      Matrix4x4F mat = { 1, 2, 2, 0,
                         2, 2, -2, 0,
                         3, 3, 3, 0,
                         4, 4, 4, 1.f };
   
      auto res = pt4 * mat;
      pt4.x_ = 18;
      pt4.y_ = 19;
      pt4.z_ = 11;
      BOOST_CHECK(res == pt4);

      Matrix1x4F mat1x4 = {1,2,3,1};
      auto res3 = mat1x4 * mat;
      BOOST_CHECK_EQUAL(res3.at(0,0), 18);
      BOOST_CHECK_EQUAL(res3.at(0, 1), 19);
      BOOST_CHECK_EQUAL(res3.at(0, 2), 11);
      BOOST_CHECK_EQUAL(res3.at(0, 3), 1);
  }

  Object obj(1, "testobj");
  obj.addVertex({0,4,8});
  obj.addVertex({4, -4, 8});
  obj.addVertex({-4, -4, 8});

  obj.addPolygon({0, 1, 2});


  addToWorld(obj, 100.f, 200.f, 300.f);
  {
    auto it = obj.localVertexList_.begin();
    auto transVerit = obj.transVertexList_.begin();
    while (transVerit != obj.transVertexList_.end()) {
      const auto pt = *it;
      const auto transpt = *transVerit;
      BOOST_CHECK_EQUAL(transpt.x_, pt.x_ + 100.f);
      BOOST_CHECK_EQUAL(transpt.y_, pt.y_ + 200.f);
      BOOST_CHECK_EQUAL(transpt.z_, pt.z_ + 300.f);
      BOOST_CHECK_EQUAL(transpt.w_, 1.f);

      ++it;
      ++transVerit;
    }
  }

  float anglex = 10.f;
  float angley = 20.f;
  float anglez = 30.f;

  setCamera(obj, {100.f, 200.f, 100.f}, anglex, angley, anglez);
  {
    //rotate with: YXZ
    anglex = -anglex;
    angley = -angley;
    anglez = -anglez;

    auto it = obj.localVertexList_.begin();
    auto transVerit = obj.transVertexList_.begin();
    while (it != obj.localVertexList_.end()) {
      auto pt = *it;
      const auto transpt = *transVerit;

      //ToWorld
      pt.x_ += 100.f;
      pt.y_ += 200.f;
      pt.z_ += 300.f;

      //to camera
      pt.x_ -= 100.f;
      pt.y_ -= 200.f;
      pt.z_ -= 100.f;


      float x,y,z;
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

      BOOST_CHECK_EQUAL(transpt.x_, pt.x_);
      BOOST_CHECK_EQUAL(transpt.y_, pt.y_);
      BOOST_CHECK_EQUAL(transpt.z_, pt.z_);
      BOOST_CHECK_EQUAL(transpt.w_, 1.f);

      //BOOST_CHECK(vector_impl::equalZero(transpt.x_ - pt.x_));
      //BOOST_CHECK(vector_impl::equalZero(transpt.y_ - pt.y_));
      //BOOST_CHECK(vector_impl::equalZero(transpt.z_ - pt.z_));
      //BOOST_CHECK(vector_impl::equalZero(transpt.w_ - 1.f));

      ++it;
      ++transVerit;
    }
  }



}