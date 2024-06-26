//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \file sap_announcer_tests.cc
///

#include <gtest/gtest.h>
#include <unistd.h>

#include <map>

#include "rtp/rtp.h"
#include "sap/sap_announcer.h"

TEST(SapAnnouncerTest, Active) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  ASSERT_FALSE(announcer.Active());
}

TEST(SapAnnouncerTest, StartStop) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  ASSERT_FALSE(announcer.Active());
  announcer.Start();
}
TEST(SapAnnouncerTest, MultiStartStop) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();

  announcer.Start();
  announcer.Start();
  announcer.Start();
  announcer.Start();
  announcer.Stop();
  announcer.Stop();
  announcer.Stop();
}

TEST(SapAnnouncerTest, AddSapAnnouncement) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  announcer.Start();

  announcer.DeleteAllSapAnnouncements();
  ASSERT_EQ(announcer.GetActiveStreamCount(), 0);
  mediax::rtp::StreamInformation message = {
      "HD Stream", "239.192.5.2", 5004, 1920, 1080, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  announcer.AddSapAnnouncement(message);
  ASSERT_EQ(announcer.GetActiveStreamCount(), 1);
  announcer.Stop();
}

TEST(SapAnnouncerTest, AddMultipleSAPAnnouncements) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  announcer.Start();
  announcer.DeleteAllSapAnnouncements();
  ASSERT_EQ(announcer.GetActiveStreamCount(), 0);

  mediax::rtp::StreamInformation message1 = {
      "HD Stream", "239.192.5.2", 5004, 1920, 1080, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message2 = {
      "SD Stream", "239.192.6.1", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  announcer.AddSapAnnouncement(message1);
  announcer.AddSapAnnouncement(message2);
  ASSERT_EQ(announcer.GetActiveStreamCount(), 2);
  announcer.Stop();
}

TEST(SapAnnouncerTest, DeleteAllSAPAnnouncements) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  announcer.DeleteAllSapAnnouncements();
  announcer.Start();

  announcer.DeleteAllSapAnnouncements();

  mediax::rtp::StreamInformation message1 = {
      "HD Stream", "239.192.5.2", 5004, 1920, 1080, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message2 = {
      "SD Stream 1", "239.192.6.1", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message3 = {
      "SD Stream 2", "239.192.6.2", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message4 = {
      "SD Stream 3", "239.192.6.3", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message5 = {
      "SD Stream 4", "239.192.6.4", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message6 = {
      "SD Stream 5", "239.192.6.5", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message7 = {
      "SD Stream 6", "239.192.6.6", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  announcer.AddSapAnnouncement(message1);
  announcer.AddSapAnnouncement(message2);
  announcer.AddSapAnnouncement(message3);
  announcer.AddSapAnnouncement(message4);
  announcer.AddSapAnnouncement(message5);
  announcer.AddSapAnnouncement(message6);
  announcer.AddSapAnnouncement(message7);
  ASSERT_EQ(announcer.GetActiveStreamCount(), 7);
  announcer.DeleteAllSapAnnouncements();
  ASSERT_EQ(announcer.GetActiveStreamCount(), 0);
  announcer.Stop();
}

TEST(SapAnnouncerTest, DeleteSAPAnnouncement) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  announcer.Start();
  mediax::rtp::StreamInformation message = {
      "SD Stream 6", "239.192.6.6", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  announcer.AddSapAnnouncement(message);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).deleted, false);
  announcer.DeleteSapAnnouncement(message.session_name);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).deleted, true);
  announcer.UndeleteSapAnnouncement(message.session_name);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).deleted, false);
  announcer.Stop();
}

TEST(SapAnnouncerTest, GetInterfaces) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  announcer.Start();

  std::map<uint32_t, std::string> interfaces;
  interfaces = announcer.GetInterfaces();
  ASSERT_GT(interfaces.size(), 0);
  // print the interfaces
  for (auto it = interfaces.begin(); it != interfaces.end(); ++it) {
    std::cout << it->first << " => " << it->second << '\n';
  }
  announcer.Stop();
}

TEST(SapAnnouncerTest, ReAddAnnouncment) {
  mediax::sap::SapAnnouncer &announcer = mediax::sap::SapAnnouncer::GetInstance();
  announcer.Start();
  announcer.DeleteAllSapAnnouncements();
  ASSERT_EQ(announcer.GetActiveStreamCount(), 0);

  mediax::rtp::StreamInformation message = {
      "SD Stream 6", "239.192.6.6", 5004, 1280, 720, 30, mediax::rtp::ColourspaceType::kColourspaceYuv422, false};
  mediax::rtp::StreamInformation message_updated = {
      "SD Stream 6", "255.255.255.255", 5005, 640, 480, 25, mediax::rtp::ColourspaceType::kColourspaceRgb24, true};

  announcer.AddSapAnnouncement(message);
  ASSERT_EQ(announcer.GetActiveStreamCount(), 1);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).hostname, "239.192.6.6");

  // Re-add announcment with different address
  message_updated.deleted = false;
  announcer.AddSapAnnouncement(message_updated);
  ASSERT_EQ(announcer.GetActiveStreamCount(), 2);
  // Delete the second announcment
  announcer.DeleteSapAnnouncement(message_updated.session_name, "255.255.255.255");
  ASSERT_EQ(announcer.GetActiveStreamCount(), 1);
  // Re-add the second announcment (Will undelete)
  announcer.AddSapAnnouncement(message_updated);
  ASSERT_EQ(announcer.GetActiveStreamCount(), 2);

  // Will get first match
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).hostname, message.hostname);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).port, message.port);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).width, message.width);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).height, message.height);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).framerate, message.framerate);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name).encoding, message.encoding);

  // Get first but more explicit
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name, "239.192.6.6").hostname, message.hostname);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name, "239.192.6.6").port, message.port);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name, "239.192.6.6").width, message.width);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name, "239.192.6.6").height, message.height);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name, "239.192.6.6").framerate, message.framerate);
  EXPECT_EQ(announcer.GetSapAnnouncment(message.session_name, "239.192.6.6").encoding, message.encoding);

  // To get the second message we need to use the multicast address
  EXPECT_EQ(announcer.GetSapAnnouncment(message_updated.session_name, "255.255.255.255").hostname,
            message_updated.hostname);
  EXPECT_EQ(announcer.GetSapAnnouncment(message_updated.session_name, "255.255.255.255").port, message_updated.port);
  EXPECT_EQ(announcer.GetSapAnnouncment(message_updated.session_name, "255.255.255.255").width, message_updated.width);
  EXPECT_EQ(announcer.GetSapAnnouncment(message_updated.session_name, "255.255.255.255").height,
            message_updated.height);
  EXPECT_EQ(announcer.GetSapAnnouncment(message_updated.session_name, "255.255.255.255").framerate,
            message_updated.framerate);
  EXPECT_EQ(announcer.GetSapAnnouncment(message_updated.session_name, "255.255.255.255").encoding,
            message_updated.encoding);
}