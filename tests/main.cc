///  \file main.cc

#include <gflags/gflags.h>  // for ParseCommandLineFlags
#include <glog/logging.h>
#include <gtest/gtest.h>  // for InitGoogleTest, RUN_ALL_TESTS
#include <rtp/rtp.h>
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  mediax::InitRtp(argc, argv);
  int ret = RUN_ALL_TESTS();
  mediax::RtpCleanup();
  return ret;
}