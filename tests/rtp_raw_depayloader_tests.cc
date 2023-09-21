//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \file rtp_depayloader_tests.cc
///

#include <glog/logging.h>
#include <gtest/gtest.h>
#include <unistd.h>

#include "raw/rtpvraw_depayloader.h"
#include "raw/rtpvraw_payloader.h"
#include "rtp/rtp_utils.h"
#include "util_tests.h"
#include "utils/colourspace_cpu.h"

TEST(RTPDepayloaderTest, Copy) {
  mediax::RtpvrawDepayloader rtp;
  mediax::RtpvrawDepayloader rtp2;
  rtp = rtp2;
}

TEST(RTPDepayloaderTest, Timeout) {
  uint8_t* yuv_test;

  mediax::RtpvrawDepayloader rtp;
  rtp.SetStreamInfo("test_session_name", mediax::ColourspaceType::kColourspaceRgb24, 640, 480, 25, "127.0.0.1", 5004);
  ASSERT_EQ(rtp.GetHeight(), 640);
  ASSERT_EQ(rtp.GetWidth(), 480);
  ASSERT_EQ(rtp.GetColourSpace(), mediax::ColourspaceType::kColourspaceRgb24);
  ASSERT_EQ(rtp.GetIpAddress(), "127.0.0.1");
  ASSERT_EQ(rtp.GetPort(), 5004);
  ASSERT_EQ(rtp.GetSessionName(), "test_session_name");
  rtp.Open();
  EXPECT_FALSE(rtp.Receive(&yuv_test, 80));
  rtp.Close();
}

void SendVideoCheckered(std::string ip, uint32_t height, uint32_t width, uint32_t framerate, uint32_t portno) {
  mediax::RtpvrawPayloader rtp;
  rtp.SetStreamInfo("test_session_name", mediax::ColourspaceType::kColourspaceRgb24, height, width, 25, ip, portno);
  rtp.Open();
  rtp.Start();
  // Create a buffer of 640x480x3 bytes (RGB)
  std::vector<uint8_t> buffer(640 * 480 * 3);
  CreateCheckeredTestCard(buffer.data(), 640, 480, mediax::ColourspaceType::kColourspaceRgb24);
  rtp.Transmit(buffer.data(), 80);
  rtp.Stop();
  rtp.Close();
}

TEST(RTPDepayloaderTest, UnicastOk) {
  std::array<uint8_t, 640 * 480 * 3> rgb_test;
  video::ColourSpaceCpu colourspace;
  mediax::RtpvrawDepayloader rtp;

  rtp.SetStreamInfo("test_session_name", mediax::ColourspaceType::kColourspaceRgb24, 640, 480, 25, "127.0.0.1", 5004);
  rtp.Open();
  rtp.Start();
  SendVideoCheckered("127.0.0.1", 640, 480, 30, 5004);
  uint8_t* data = rgb_test.data();
  EXPECT_TRUE(rtp.Receive(&data, 80));
  WritePngFile(rgb_test.data(), 640, 480, "UnicastOk.png");
  rtp.Stop();
  rtp.Close();
}

TEST(RTPDepayloaderTest, MulticastOk) {
  std::array<uint8_t, 640 * 480 * 3> rgb_test;
  video::ColourSpaceCpu colourspace;

  mediax::RtpvrawDepayloader rtp;
  rtp.SetStreamInfo("test_session_name", mediax::ColourspaceType::kColourspaceRgb24, 640, 480, 25, "239.192.1.200",
                    5004);
  rtp.Open();
  rtp.Start();
  SendVideoCheckered("239.192.1.200", 640, 480, 30, 5004);
  uint8_t* data = rgb_test.data();
  EXPECT_TRUE(rtp.Receive(&data, 80));
  WritePngFile(rgb_test.data(), 640, 480, "MulticastOk.png");
  rtp.Stop();
  rtp.Close();
}

TEST(RTPDepayloaderTest, Many) {
  std::array<std::string, 10> ip_pool = {"239.192.1.1", "239.192.1.2", "239.192.1.3", "239.192.1.4", "239.192.1.5",
                                         "239.192.1.6", "239.192.1.7", "239.192.1.8", "239.192.1.9", "239.192.1.10"};
  std::array<mediax::RtpvrawDepayloader, 10> rtp;
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << "Creating stream number " << i << " with IP:" << ip_pool[i];
    rtp[i].SetStreamInfo("test_session_name_" + std::to_string(i), mediax::ColourspaceType::kColourspaceRgb24, 640, 480,
                         25, ip_pool[i], 5004);
  }

  for (int i = 0; i < 10; i++) {
    LOG(INFO) << "Opening stream number " << rtp[i].GetIpAddress() << "\n";
    rtp[i].Open();
  }
}

TEST(RTPDepayloaderTest, ReOpening) {
  // Open stream 10 times
  for (int i = 0; i < 10; i++) {
    mediax::RtpvrawDepayloader rtp;
    rtp.SetStreamInfo("test_session_name", mediax::ColourspaceType::kColourspaceYuv, 640, 480, 25, "127.0.0.1");
    rtp.Open();
    rtp.Start();
    rtp.Stop();
    rtp.Close();
  }
}

void OpenStream(std::string ipaddr, uint32_t height, uint32_t width, uint32_t framerate, uint32_t portno) {
  std::vector<uint8_t> yuv_test;
  mediax::RtpvrawDepayloader rtp;
  yuv_test.resize(height * width * 2);
  rtp.SetStreamInfo("test", mediax::ColourspaceType::kColourspaceYuv, 640, 480, 25, ipaddr);
  rtp.Open();
  rtp.Start();

  bool running = true;
  int frame_count = 0;
  // Recieve video
  while (running) {
    uint8_t* data = yuv_test.data();
    bool ret = rtp.Receive(&data, 0);
    EXPECT_TRUE(ret);
    if (ret == false) break;
    if (frame_count++ > 10) running = false;
  }

  rtp.Stop();
  rtp.Close();
}

std::string ExecuteCommand(std::string bash) {
  std::string result = "";
  FILE* pipe = popen(bash.c_str(), "r");
  if (!pipe) throw std::runtime_error("popen() failed!");
  try {
    while (!feof(pipe)) {
      char buf[256];
      if (fgets(buf, 256, pipe) != NULL) result += buf;
    }
  } catch (...) {
    pclose(pipe);
    throw;
  }
  pclose(pipe);
  return result;
}

TEST(RTPDepayloaderTest, DISABLED_SwitchStreams) {
  // Start five streams run the bash script start_five_streams.sh
  ExecuteCommand("../scripts/start_five_streams.sh");
  // Switch between 5 streams
  std::cout << "Opening stream 1" << std::endl;
  OpenStream("239.192.3.1", 640, 480, 30, 5004);
  std::cout << "Opening stream 2" << std::endl;
  OpenStream("239.192.3.2", 640, 480, 30, 5004);
  std::cout << "Opening stream 3" << std::endl;
  OpenStream("239.192.3.3", 640, 480, 30, 5004);
  std::cout << "Opening stream 4" << std::endl;
  OpenStream("239.192.3.4", 640, 480, 30, 5004);
  std::cout << "Opening stream 5" << std::endl;
  OpenStream("239.192.3.5", 640, 480, 30, 5004);
  ExecuteCommand("pkill -f gst-launch-1.0");
}