//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \brief RTP streaming video class for H.264 DEF-STAN 00-82 video streams
///
/// \file rtph_264_payloader.cc
///

#include "h265/gst/vaapi/rtp_h265_payloader.h"

#include <glog/logging.h>
#include <gst/app/gstappsrc.h>
#include <gst/gst.h>

#include "h265/gst/vaapi/rtp_h265_depayloader.h"

namespace mediax::rtp::h265::gst::vaapi {

RtpH265GstVaapiPayloader::RtpH265GstVaapiPayloader() = default;

RtpH265GstVaapiPayloader::~RtpH265GstVaapiPayloader() = default;

void RtpH265GstVaapiPayloader::SetStreamInfo(const ::mediax::rtp::StreamInformation &stream_information) {
  egress_.encoding = stream_information.encoding;
  egress_.height = stream_information.height;
  egress_.width = stream_information.width;
  egress_.framerate = stream_information.framerate;
  egress_.name = stream_information.session_name;
  egress_.hostname = stream_information.hostname;
  egress_.port_no = stream_information.port;
  egress_.settings_valid = true;
}

int RtpH265GstVaapiPayloader::Transmit(unsigned char *new_buffer, bool timeout) {
  if (!started_) {
    DLOG(ERROR) << "RTP H.264 payloader not started";
    return -1;
  }

  // Gstreamer send appsrc
  // Send a frame
  GstElement *appsrc = gst_bin_get_by_name(GST_BIN(pipeline_), "rtp-h265-appsrc");
  int size = egress_.width * egress_.height * 3;
  GstBuffer *buffer = gst_buffer_new_allocate(nullptr, size, nullptr);

  // Define info
  GstMapInfo info;
  gst_buffer_map(buffer, &info, GST_MAP_WRITE);

  // Copy some data
  memcpy(info.data, new_buffer, size);
  gst_buffer_unmap(buffer, &info);

  // Set the buffer timestamp
  GST_BUFFER_PTS(buffer) = GST_CLOCK_TIME_NONE;
  GST_BUFFER_DTS(buffer) = GST_CLOCK_TIME_NONE;

  // Push the buffer to the appsrc and emit signal
  gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer);

  // Release the appsrc
  gst_object_unref(appsrc);

  return 0;
}

bool RtpH265GstVaapiPayloader::Open() {
  // Setup a gstreamer pipeline to decode H.264 with Intel VAAPI

  // Create a pipeline
  pipeline_ = gst_pipeline_new("rtp-h265-pipeline");
  // Create a custom appsrc element to receive the H.264 stream
  GstElement *appsrc = gst_element_factory_make("appsrc", "rtp-h265-appsrc");
  g_object_set(G_OBJECT(appsrc), "stream-type", 0, "format", GST_FORMAT_TIME, nullptr);

  // Create a capsfilter element to set the caps for the H.264 stream
  GstElement *capsfilter = gst_element_factory_make("capsfilter", "rtp-h265-capsfilter");
  // Raw RGB caps
  GstCaps *caps =
      gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING, "RGB", "width", G_TYPE_INT, egress_.width, "height",
                          G_TYPE_INT, egress_.height, "framerate", GST_TYPE_FRACTION, egress_.framerate, 1, nullptr);
  g_object_set(G_OBJECT(capsfilter), "caps", caps, nullptr);

  // Convert the video colourspace
  GstElement *videoconvert = gst_element_factory_make("videoconvert", "rtp-h265-videoconvert");

  // Create a vaapih265enc element to decode the H.264 stream
  GstElement *vaapih265enc = gst_element_factory_make("vaapih265enc", "rtp-h265-vaapih265enc");

  // RTP payloader
  GstElement *rtp264pay = gst_element_factory_make("rtph265pay", "rtp-h265-payloader");

  // Create a udpsink element to stream over ethernet
  GstElement *udpsink = gst_element_factory_make("udpsink", "rtp-h265-udpsink");
  g_object_set(G_OBJECT(udpsink), "host", egress_.hostname.c_str(), "port", egress_.port_no, nullptr);

  // Add all elements to the pipeline
  gst_bin_add_many(GST_BIN(pipeline_), appsrc, capsfilter, videoconvert, vaapih265enc, rtp264pay, udpsink, nullptr);

  // Link the elements
  gst_element_link_many(appsrc, capsfilter, videoconvert, vaapih265enc, rtp264pay, udpsink, nullptr);

  return true;
}

void RtpH265GstVaapiPayloader::Close() {
  // Stop the pipeline
  Stop();

  // Free resources
  gst_object_unref(pipeline_);
}

void RtpH265GstVaapiPayloader::Start() {
  started_ = true;
  // Start the pipeline
  gst_element_set_state(pipeline_, GST_STATE_PLAYING);
}

void RtpH265GstVaapiPayloader::Stop() {
  started_ = false;
  // Stop the pipeline
  gst_element_set_state(pipeline_, GST_STATE_NULL);
}

}  // namespace mediax::rtp::h265::gst::vaapi