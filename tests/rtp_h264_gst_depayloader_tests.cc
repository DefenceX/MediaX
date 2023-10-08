//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \file rtp_h264_depayloader_tests.cc
///

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#include "h264/gst/vaapi/rtp_h264_depayloader.h"
#include "rtp/rtp_utils.h"
#include "uncompressed/rtp_uncompressed_payloader.h"
#include "util_tests.h"
#include "utils/colourspace_cpu.h"

TEST(RtpH264DepayloaderTest, Timeout) {
#if !GST_SUPPORTED
  GTEST_SKIP();
#endif

  uint8_t* rgb_test;

  std::shared_ptr<mediax::RtpDepayloader> rtp;
  rtp = std::make_shared<mediax::h264::gst::vaapi::RtpH264Depayloader>();
  rtp->SetSessionName("test_session_name");
  ASSERT_EQ(rtp->GetSessionName(), "test_session_name");
  rtp->Open();
  EXPECT_FALSE(rtp->Receive(&rgb_test, 80));
  rtp->Close();
}

TEST(RtpH264DepayloaderTest, UnicastOk) {
#if !GST_SUPPORTED
  GTEST_SKIP();
#endif

  std::array<uint8_t, 1280 * 720 * 3> rgb_test;
  mediax::video::ColourSpaceCpu colourspace;
  mediax::h264::gst::vaapi::RtpH264Depayloader rtp;

  // Increase GST debug level
  setenv("GST_DEBUG", "3", 1);
  // Set the stream details individually
  rtp.SetIpAddress("127.0.0.1");
  rtp.SetPort(5004);
  rtp.SetSessionName("test_session_name");
  rtp.SetHeight(720);
  rtp.SetWidth(1280);
  rtp.SetColourSpace(::mediax::ColourspaceType::kColourspaceH264Part10);
  std::cout << "Setup" << std::endl;

  // Start the stream
  EXPECT_TRUE(rtp.Open());
  std::cout << "Opened" << std::endl;
  rtp.Start();
  std::cout << "Started" << std::endl;
  uint8_t* data = rgb_test.data();
  EXPECT_FALSE(rtp.Receive(&data, 80));
  rtp.Stop();
  rtp.Close();

  EXPECT_EQ(rtp.GetColourSpace(), ::mediax::ColourspaceType::kColourspaceH264Part10);
  mediax::video::ColourSpaceCpu convert;
  convert.Nv12ToRgb(rtp.GetHeight(), rtp.GetWidth(), data, rgb_test.data());

  EXPECT_EQ(rtp.GetHeight(), 720);
  EXPECT_EQ(rtp.GetWidth(), 1280);

  std::cout << "Received" << std::endl;
  WritePngFile(rgb_test.data(), rtp.GetWidth(), rtp.GetHeight(), "H264_Image.png");
}

TEST(RtpH264DepayloaderTest, UnicastOkSetStreamInfo) {
#if !GST_SUPPORTED
  GTEST_SKIP();
#endif

  std::array<uint8_t, 1280 * 720 * 3> rgb_test;
  mediax::video::ColourSpaceCpu colourspace;
  mediax::h264::gst::vaapi::RtpH264Depayloader rtp;

  // Increase GST debug level
  setenv("GST_DEBUG", "3", 1);
  // Set the stream details using set stream info
  ::mediax::StreamInformation stream_info = {.session_name = "test_session_name",
                                             .hostname = "127.0.0.1",
                                             .port = 5004,
                                             .height = 720,
                                             .width = 1280,
                                             .framerate = 25,
                                             .encoding = ::mediax::ColourspaceType::kColourspaceH264Part10,
                                             .deleted = false};
  rtp.SetStreamInfo(stream_info);
  std::cout << "Setup" << std::endl;

  // Start the stream
  EXPECT_TRUE(rtp.Open());
  std::cout << "Opened" << std::endl;
  rtp.Start();
  std::cout << "Started" << std::endl;
  uint8_t* data = rgb_test.data();
  EXPECT_FALSE(rtp.Receive(&data, 80));
  rtp.Stop();
  rtp.Close();

  EXPECT_EQ(rtp.GetColourSpace(), ::mediax::ColourspaceType::kColourspaceH264Part10);
  mediax::video::ColourSpaceCpu convert;
  convert.Nv12ToRgb(rtp.GetHeight(), rtp.GetWidth(), data, rgb_test.data());

  EXPECT_EQ(rtp.GetHeight(), 720);
  EXPECT_EQ(rtp.GetWidth(), 1280);

  std::cout << "Received" << std::endl;
  WritePngFile(rgb_test.data(), rtp.GetWidth(), rtp.GetHeight(), "H264_Image.png");
}
