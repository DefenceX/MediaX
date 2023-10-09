
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
/// Below is a sample pipeline to create video streams using GStreamer:
/// \code
/// gst-launch-1.0 -v udpsrc caps="application/x-rtp, media=(string)video, clock-rate=(int)90000,
/// encoding-name=(string)H264" ! rtph264depay ! h264parse ! queue ! vaapih264dec ! caps="video/x-raw, format=RGB" !
/// videoconvert ! appsink
/// \endcode
///
/// \file rtph264_depayloader.cc
///

#include "h264/gst/vaapi/rtp_h264_depayloader.h"

#include <glog/logging.h>
#include <gst/gst.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "rtp/rtp_utils.h"

namespace mediax::h264::gst::vaapi {

RtpH264Depayloader::~RtpH264Depayloader() = default;

void RtpH264Depayloader::SetStreamInfo(const ::mediax::StreamInformation &stream_information) {
  ingress_.encoding = stream_information.encoding;
  ingress_.height = stream_information.height;
  ingress_.width = stream_information.width;
  ingress_.framerate = stream_information.framerate;
  ingress_.name = stream_information.session_name;
  ingress_.hostname = stream_information.hostname;
  ingress_.port_no = stream_information.port;
  ingress_.settings_valid = true;
}

GstFlowReturn NewFrameCallback(GstAppSink *appsink, gpointer user_data) {
  gint width = 0;
  gint height = 0;
  auto depayloader = static_cast<RtpH264Depayloader *>(user_data);

  // Pull the sample from the appsink
  GstSample *sample = gst_app_sink_pull_sample(appsink);

  // Get the buffer from the sample
  GstBuffer *buffer = gst_sample_get_buffer(sample);

  // Get the size of the buffer
  gsize size = gst_buffer_get_size(buffer);

  // Allocate memory for the frame data
  // guint8 *data = g_new(guint8, size);
  depayloader->GetBuffer().resize(size);

  // Get the buffer height and width
  const GstCaps *caps = gst_sample_get_caps(sample);
  const GstStructure *structure = gst_caps_get_structure(caps, 0);
  gst_structure_get_int(structure, "height", &height);
  gst_structure_get_int(structure, "width", &width);
  // Get the colourspace
  const gchar *colorspace = gst_structure_get_string(structure, "format");

  // Set the ColourspaceType

  if (strncmp(colorspace, "UYVY", 4) == 0) {
    depayloader->SetColourSpace(ColourspaceType::kColourspaceYuv);
  } else if (strncmp(colorspace, "RGB", 3) == 0) {
    depayloader->SetColourSpace(ColourspaceType::kColourspaceRgb24);
  } else if (strncmp(colorspace, "NV12", 3) == 0) {
    depayloader->SetColourSpace(ColourspaceType::kColourspaceNv12);
  } else if (strncmp(colorspace, "RGBA", 4) == 0) {
    depayloader->SetColourSpace(ColourspaceType::kColourspaceRgba);
  } else {
    depayloader->SetColourSpace(ColourspaceType::kColourspaceUndefined);
    DLOG(WARNING) << "Unknown colourspace " << colorspace;
  }
  depayloader->SetHeight(height);
  depayloader->SetWidth(width);

  // Get a pointer to the video frame
  GstMapInfo map;
  gst_buffer_map(buffer, &map, GST_MAP_READ);

  // Copy the frame data
  std::copy(map.data, map.data + size, depayloader->GetBuffer().begin());

  // Unmap the buffer
  gst_buffer_unmap(buffer, &map);

  // Release the sample
  gst_sample_unref(sample);

  // Set good frame flag
  depayloader->NewFrame();

  return GST_FLOW_OK;
}

bool RtpH264Depayloader::Open() {
  // Open the pipeline
  // Create a pipeline
  pipeline_ = gst_pipeline_new("rtp-h264-pipeline");

  // Create a udpsrc element to receive the RTP stream
  GstElement *udpsrc = gst_element_factory_make("udpsrc", "rtp-h264-udpsrc");
  g_object_set(G_OBJECT(udpsrc), "port", GetPort(), nullptr);

  // Create a capsfilter element to set the caps for the RTP stream
  GstElement *capsfilter = gst_element_factory_make("capsfilter", "rtp-h264-capsfilter");
  GstCaps *caps =
      gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264");
  g_object_set(G_OBJECT(capsfilter), "caps", caps, nullptr);
  gst_caps_unref(caps);

  // Create a rtph264depay element to depayload the RTP stream
  GstElement *rtph264depay = gst_element_factory_make("rtph264depay", "rtp-h264-depay");

  // H.264 parse
  GstElement *h264parse = gst_element_factory_make("h264parse", "rtp-h264-h264parse");

  // Queue
  GstElement *queue = gst_element_factory_make("queue", "rtp-h264-queue");

  // Decode frame using vaapi
  GstElement *vaapih264dec = gst_element_factory_make("vaapih264dec", "rtp-h264-vaapih264dec");

  // Create a custom appsrc element to receive the H.264 stream
  GstElement *appsink = gst_element_factory_make("appsink", "rtp-h264-appsrc");
  // Set the callback function for the appsink
  GstAppSinkCallbacks callbacks = {.new_sample = NewFrameCallback};
  gst_app_sink_set_callbacks(GST_APP_SINK(appsink), &callbacks, this, nullptr);

  // Add all elements to the pipeline
  gst_bin_add_many(GST_BIN(pipeline_), udpsrc, capsfilter, rtph264depay, h264parse, queue, vaapih264dec, appsink,
                   nullptr);

  // Link the elements
  gst_element_link_many(udpsrc, capsfilter, rtph264depay, h264parse, queue, vaapih264dec, appsink, nullptr);

  return true;
}

void RtpH264Depayloader::Start() {
  // Start the pipeline
  gst_element_set_state(pipeline_, GST_STATE_PLAYING);
}

void RtpH264Depayloader::Stop() {
  // Stop the pipeline
  gst_element_set_state(pipeline_, GST_STATE_NULL);
  std::cout << "Pipeline stopped" << std::endl;
  // Wait for the pipeline to finish
  GstBus *bus = gst_element_get_bus(pipeline_);

  // Free resources
  gst_object_unref(bus);
}

void RtpH264Depayloader::Close() {
  // Destroy the pipeline
  gst_object_unref(pipeline_);
}

bool RtpH264Depayloader::Receive(uint8_t **cpu, int32_t timeout) {
  auto start_time = std::chrono::high_resolution_clock::now();

  while (!new_rx_frame_) {
    // Check timeout
    if (auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
        elapsed > std::chrono::milliseconds(timeout)) {
      return false;
    }
    // Sleep 1ms and wait for a new frame
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  // Dont start a new thread if a frame is available just return it
  *cpu = buffer_in_.data();

  new_rx_frame_ = false;
  return true;
}

std::vector<uint8_t> &RtpH264Depayloader::GetBuffer() { return buffer_in_; }

void RtpH264Depayloader::NewFrame() { new_rx_frame_ = true; }

}  // namespace mediax::h264::gst::vaapi
