// Copyright (c) 2020 Robotics and Artificial Intelligence Lab, KAIST
//
// Any unauthorized copying, alteration, distribution, transmission,
// performance, display or use of this material is prohibited.
//
// All rights reserved.

#include <gtest/gtest.h>

#include <string>

#include "../include/raisin_parameter/parameter_container.hpp"

//
// Tests
//

class Child {
 public:
  explicit Child(raisin::parameter::ParameterContainer& param): param_(param["Child"]) {
    param_("param1").setOptions({"hello", "goodbye"});
    param_("param1") = "hello";
    Eigen::VectorXd testVec(3);
    testVec << 1, 2, 3;
    param_("param2") = testVec;
    std::vector<std::string> testVecString = {"hello", "world"};
    param_("param3") = testVecString;
    std::vector<int> testVecInt = {1, 2, 3};
    param_("param4") = testVecInt;
  }

 private:
  raisin::parameter::ParameterContainer& param_;
};

class Parent {
 public:
  Parent()
      : param_(raisin::parameter::ParameterContainer::getRoot()),
        child_(raisin::parameter::ParameterContainer::getRoot()) {
    param_("param1") = 3;
    param_("param2") = true;
  }

 private:
  raisin::parameter::ParameterContainer& param_;
  Child child_;
};

TEST(ParameterTest, ParameterTest1) {
  Parent parent;
  auto& rootParamContainer = raisin::parameter::ParameterContainer::getRoot();
  EXPECT_EQ("hello", std::string(rootParamContainer["Child"]("param1")));
  EXPECT_EQ(3, int(rootParamContainer("param1")));
  EXPECT_EQ(true, bool(rootParamContainer("param2")));
  Eigen::VectorXd testVec = rootParamContainer["Child"]("param2");
  EXPECT_EQ(6, testVec.sum());
  std::vector<std::string> testVecString = rootParamContainer["Child"]("param3");
  EXPECT_EQ("hello", testVecString[0]);
  EXPECT_EQ("world", testVecString[1]);
  std::vector<int> testVecInt = rootParamContainer["Child"]("param4");
  EXPECT_EQ(6, testVecInt[0] + testVecInt[1] + testVecInt[2]);

  rootParamContainer["file"].loadFromPackageParameterFile("raisin_parameter", "params.yaml");

  EXPECT_EQ(5.3, double(rootParamContainer["file"]("test_double")));
  EXPECT_EQ("hello", std::string(rootParamContainer["file"]("test_string")));
  EXPECT_EQ(3, int(rootParamContainer["file"]("test_int")));
  EXPECT_EQ(true, bool(rootParamContainer["file"]("test_bool")));
  Eigen::VectorXd test_vec_true(3), test_vec_read(3);
  test_vec_true << 1, 2, 3;
  test_vec_read = rootParamContainer["file"]("test_vec_double");
  EXPECT_NEAR((test_vec_true - test_vec_read).sum(), 0, 1e-15);
  std::vector<double> test_vec_double_true = {1, 2, 3};
  std::vector<double> test_vec_double_read = rootParamContainer["file"]("test_vec_double");
  for (int i = 0; i < 3; i++) {
    EXPECT_NEAR(test_vec_double_true[i], test_vec_double_read[i], 1e-15);
  }
  std::vector<std::string> test_vec_string_true = {"hello", "world"};
  std::vector<std::string> test_vec_string_read = rootParamContainer["file"]("test_vec_string");
  for (int i = 0; i < 2; i++) {
    EXPECT_EQ(test_vec_string_true[i], test_vec_string_read[i]);
  }
  std::vector<int> test_vec_int_true = {1, 2, 3};
  std::vector<int> test_vec_int_read = rootParamContainer["file"]("test_vec_int");
  for (int i = 0; i < 3; i++) {
    EXPECT_EQ(test_vec_int_true[i], test_vec_int_read[i]);
  }
}
