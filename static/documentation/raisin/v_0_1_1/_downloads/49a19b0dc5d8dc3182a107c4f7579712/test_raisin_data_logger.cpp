// Copyright (c) 2020 Robotics and Artificial Intelligence Lab, KAIST
//
// Any unauthorized copying, alteration, distribution, transmission,
// performance, display or use of this material is prohibited.
//
// All rights reserved.

// gtest
#include <gtest/gtest.h>

// std
#include <string>
#include <vector>

// ros

// raisin
#include "../include/raisin_data_logger/raisin_data_logger.hpp"

//
// Tests
//

TEST(ParameterTest, ParameterTest1) {
  /// initialize logging variables
  double dDouble1 = 1.5, dDouble2 = 2.5;
  float dFloat1 = 2.3f, dFloat2 = 4.2f;
  bool dBool1 = true, dBool2 = false;
  std::string dString1 = "hello", dString2 = "hi";
  Eigen::MatrixXd dEigen1(20, 10);
  Eigen::MatrixXf dEigen2(3, 2);
  Eigen::Matrix3f dEigen3;
  dEigen1.setConstant(2);
  dEigen2.setConstant(2);
  dEigen3.setConstant(3);
  std::vector<double> dSV = {1, 1, 1};
  std::vector<float> fFV = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
  raisim::Mat<3, 3> rotMat;
  raisim::VecDyn gc(6);
  gc.e() << 0, 1, 2, 3, 4, 5;
  rotMat.setIdentity();

  /// create this log file one in the main function
  raisin::DataLogger::getInstance().createALogFile("test", "/tmp");

  /// initialize a set of
  size_t logIdx1 = raisin::DataLogger::getInstance().initializeAnotherDataGroup(
    "raisin_datalogger_test", "life", dDouble1, "dead", dBool1, "killed", dBool2, "name", dString1,
    "name2", dString2, "mana", dFloat2, "massMat", dEigen1, "bias", dEigen3, "coolTime", dFloat1,
    "dSV", dSV, "fFV", fFV, "gc", gc, "rotMat", rotMat);

  /// initialize a set of
  size_t logIdx2 = raisin::DataLogger::getInstance().initializeAnotherDataGroup(
    "raisin_datalogger_test2", "life", dDouble1, "dead", dBool1, "killed", dBool2);

  raisin::DataLogger::getInstance().setAllowedDataSize(20000000);

  for (int i = 0; i < 71300; i++) {
    auto start = std::chrono::system_clock::now();
    raisin::DataLogger::getInstance().append(logIdx1, dDouble1, dBool1, dBool2, dString1, dString2,
      dFloat2, dEigen1, dEigen3, dFloat1, dSV, fFV, gc, rotMat);

    if (i % 2 == 0) {
      raisin::DataLogger::getInstance().append(logIdx2, dDouble1, dBool1, dBool2);
    }

    auto end = std::chrono::system_clock::now();
    auto elapsedTime = (end - start).count();
    usleep(200);
    // std::cout<<"elapsed time is " << elapsedTime<<std::endl;
    RSFATAL_IF(
      elapsedTime > 1000000, "took too long to save. ellapsed time (us) is " << elapsedTime)
  }

  raisin::DataLogger::getInstance().flush();
  RSINFO("reading " << raisin::DataLogger::getInstance().getDataDirectory())
  raisin::DataReader file(raisin::DataLogger::getInstance().getDataDirectory());

  auto& dataSet = file.getDataSet("raisin_datalogger_test");
  auto data = dataSet.data;

  auto& dataSet2 = file.getDataSet("raisin_datalogger_test2");
  auto data2 = dataSet2.data;

  EXPECT_EQ(1.5, std::stod(data["life"][0]));
  EXPECT_EQ(true, bool(std::stod(data["dead"][0])));
  EXPECT_EQ(false, bool(std::stod(data["killed"][2])));
  EXPECT_EQ(4.2, std::stod(data["mana"][0]));
  EXPECT_EQ(2.0, std::stod(data["massMat_2_3"][6]));
  EXPECT_EQ(3., std::stod(data["bias_2_1"][4]));
  EXPECT_EQ(2.3, std::stod(data["coolTime"][3]));
  EXPECT_EQ("hello", data["name"][5]);
  EXPECT_EQ(1., std::stod(data["dSV_2"][3]));
  EXPECT_EQ(2., std::stod(data["fFV_4"][1]));
  EXPECT_EQ(1., std::stod(data["rotMat_2_2"][1]));
  EXPECT_EQ(3., std::stod(data["gc_3"][1]));

  EXPECT_EQ(1.5, std::stod(data2["life"][0]));

  file.exportToCsv();
}
