
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
/// gst-launch-1.0 udpsrc port=5004 address=239.192.2.254 caps="application/x-rtp, media=(string)video,
/// clock-rate=(int)90000, encoding-name=(string)JPEG" ! rtpjpegdepay ! jpegparse ! queue ! jpegdec  ! videoconvert !
/// xvimagesink
/// \endcode
///
/// \file rtp_jpeg_depayloader.cc
///

#include "jpeg/gst/rtp_jpeg_depayloader.h"

#include <glog/logging.h>
#include <gst/gst.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "rtp/rtp_utils.h"

namespace mediax::rtp::jpeg::gst {

RtpJpegGstDepayloader::RtpJpegGstDepayloader() : mediax::rtp::RtpDepayloader() {
  // Set this for empty video buffers
  SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceRgb24);
}

RtpJpegGstDepayloader::~RtpJpegGstDepayloader() = default;

RtpJpegGstDepayloader &RtpJpegGstDepayloader::operator=(const RtpJpegGstDepayloader &other [[maybe_unused]]) {
  return *this;
}

void RtpJpegGstDepayloader::SetStreamInfo(const ::mediax::rtp::StreamInformation &stream_information) {
  ::mediax::rtp::RtpPortType &stream = GetStream();
  stream.encoding = stream_information.encoding;
  stream.height = stream_information.height;
  stream.width = stream_information.width;
  stream.framerate = stream_information.framerate;
  stream.name = stream_information.session_name;
  stream.hostname = stream_information.hostname;
  stream.port_no = stream_information.port;
  stream.settings_valid = true;
  SetSessionName(stream_information.session_name);
  SetHeight(stream_information.height);
  SetWidth(stream_information.width);
  SetBufferSize(stream_information.height * stream_information.width * 3);
  SetFramerate(stream_information.framerate);
  SetIpAddress(stream_information.hostname);
  SetPort(stream_information.port);
  SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceRgb24);
}

GstFlowReturn RtpJpegGstDepayloader::NewFrameCallback(GstAppSink *appsink, gpointer user_data) {
  gint width = 0;
  gint height = 0;
  auto depayloader = static_cast<RtpJpegGstDepayloader *>(user_data);

  // Pull the sample from the appsink
  GstSample *sample = gst_app_sink_pull_sample(appsink);

  // Get the buffer from the sample
  GstBuffer *buffer = gst_sample_get_buffer(sample);

  // Get the size of the buffer
  gsize size = gst_buffer_get_size(buffer);

  // Allocate memory for the frame data
  depayloader->GetBuffer().resize(size);

  // Get the buffer height and width
  const GstCaps *caps = gst_sample_get_caps(sample);

  const GstStructure *structure = gst_caps_get_structure(caps, 0);

  gst_structure_get_int(structure, "height", &height);
  gst_structure_get_int(structure, "width", &width);
  // Set the ColourspaceType
  if (const gchar *colorspace = gst_structure_get_string(structure, "format"); strncmp(colorspace, "UYVY", 4) == 0) {
    depayloader->SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceYuv422);
  } else if (strncmp(colorspace, "RGB", 3) == 0) {
    depayloader->SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceRgb24);
  } else if (strncmp(colorspace, "NV12", 3) == 0) {
    depayloader->SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceNv12);
  } else if (strncmp(colorspace, "RGBA", 4) == 0) {
    depayloader->SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceRgba);
  } else {
    depayloader->SetColourSpace(mediax::rtp::ColourspaceType::kColourspaceUndefined);
    DLOG(WARNING) << "Unknown colourspace " << colorspace;
    std::cerr << "Unknown format " << colorspace << " height " << height << " width " << width << " size " << size
              << "\n";  // NOLINT
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

bool RtpJpegGstDepayloader::Open() {
  // Call the base class
  RtpDepayloader::Open();

  // Create a pipeline
  pipeline_ = gst_pipeline_new("rtp-jpeg-pipeline");

  // Create a udpsrc element to receive the RTP stream
  GstElement *udpsrc = gst_element_factory_make("udpsrc", "rtp-jpeg-udpsrc");
  g_object_set(G_OBJECT(udpsrc), "port", GetPort(), nullptr);

  if (IsMulticast(GetIpAddress())) {
    g_object_set(G_OBJECT(udpsrc), "address", GetIpAddress().c_str(), nullptr);
  }

  // Create a capsfilter element to set the caps for the RTP stream
  GstElement *capsfilter = gst_element_factory_make("capsfilter", "rtp-jpeg-capsfilter");
  GstCaps *caps =
      gst_caps_from_string("application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)JPEG");
  g_object_set(G_OBJECT(capsfilter), "caps", caps, nullptr);
  gst_caps_unref(caps);

  // Create a rtpjpegdepay element to depayload the RTP stream
  GstElement *rtpjpegdepay = gst_element_factory_make("rtpjpegdepay", "rtp-jpeg-depay");

  // H.264 parse
  GstElement *jpegparse = gst_element_factory_make("jpegparse", "rtp-jpeg-jpegparse");

  // Queue
  GstElement *queue = gst_element_factory_make("queue", "rtp-jpeg-queue");

  // Decode frame using
  GstElement *jpegdec = gst_element_factory_make("jpegdec", "rtp-jpeg-jpegdec");

  // Video Convert
  GstElement *videoconvert = gst_element_factory_make("videoconvert", "rtp-jpeg-videoconvert");

  // To RGB
  GstElement *capsfilter2 = gst_element_factory_make("capsfilter", "rtp-jpeg-capsfilter2");
  GstCaps *caps2 = gst_caps_from_string("video/x-raw, format=RGB");
  g_object_set(G_OBJECT(capsfilter2), "caps", caps2, nullptr);

  // Create a custom appsrc element to receive the H.264 stream
  GstElement *appsink = gst_element_factory_make("appsink", "rtp-jpeg-appsrc");
  // Set the callback function for the appsink
  GstAppSinkCallbacks callbacks = {.new_sample = RtpJpegGstDepayloader::NewFrameCallback};
  gst_app_sink_set_callbacks(GST_APP_SINK(appsink), &callbacks, this, nullptr);

  // Add all elements to the pipeline
  gst_bin_add_many(GST_BIN(pipeline_), udpsrc, capsfilter, rtpjpegdepay, jpegparse, queue, jpegdec, videoconvert,
                   capsfilter2, appsink, nullptr);

  // Link the elements
  gst_element_link_many(udpsrc, capsfilter, rtpjpegdepay, jpegparse, queue, jpegdec, videoconvert, capsfilter2, appsink,
                        nullptr);

  return true;
}

void RtpJpegGstDepayloader::Start() {
  // Return if not open
  if (GetState() == ::mediax::rtp::StreamState::kClosed) {
    std::cerr << "Stream not open so cant be started"
              << "\n";
    return;
  }

  if (GetState() == ::mediax::rtp::StreamState::kStarted) {
    return;
  }
  // Call the base class
  RtpDepayloader::Start();
  // Start the pipeline
  gst_element_set_state(pipeline_, GST_STATE_PLAYING);
}

void RtpJpegGstDepayloader::Stop() {
  if (GetState() != ::mediax::rtp::StreamState::kStarted) {
    return;
  }
  // Call the base class
  RtpDepayloader::Stop();
  // Stop the pipeline
  gst_element_set_state(pipeline_, GST_STATE_NULL);
  // Wait for the pipeline to finish
  GstBus *bus = gst_element_get_bus(pipeline_);

  // Free resources
  gst_object_unref(bus);
}

void RtpJpegGstDepayloader::Close() {
  if (GetState() != ::mediax::rtp::StreamState::kStopped) {
    Stop();
  }

  if (GetState() == ::mediax::rtp::StreamState::kClosed) {
    return;
  }

  // Call the base class
  RtpDepayloader::Close();

  // Wait for the pipeline to finish
  GstBus *bus = gst_element_get_bus(pipeline_);

  // Free resources
  gst_object_unref(bus);

  // Destroy the pipeline
  gst_object_unref(pipeline_);
}

bool RtpJpegGstDepayloader::Receive(mediax::rtp::RtpFrameData *data, int32_t timeout) {
  auto start_time = std::chrono::high_resolution_clock::now();

  data->resolution.height = GetHeight();
  data->resolution.width = GetWidth();
  data->encoding = GetColourSpace();
  data->cpu_buffer = GetBuffer().data();
  while (!new_rx_frame_) {
    // Check timeout
    if (auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
        elapsed > std::chrono::milliseconds(timeout)) {
      // Blank the buffer, no data
      memset(buffer_in_.data(), 0, buffer_in_.size());
      return false;
    }

    // Sleep 1ms and wait for a new frame
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  new_rx_frame_ = false;
  return true;
}

void RtpJpegGstDepayloader::Callback(::mediax::rtp::RtpFrameData frame) const {
  GetCallback()(static_cast<const RtpDepayloader &>(*this), frame);
}

void RtpJpegGstDepayloader::NewFrame() {
  new_rx_frame_ = true;
  if (CallbackRegistered()) {
    RtpFrameData arg_tx = {{GetHeight(), GetWidth()}, GetBuffer().data(), GetColourSpace()};
    Callback(arg_tx);
  }
}

}  // namespace mediax::rtp::jpeg::gst
