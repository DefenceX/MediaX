//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \file rtp_h264_payloader.h

#ifndef H264_GST_VAAPI_RTP_H264_PAYLOADER_H_
#define H264_GST_VAAPI_RTP_H264_PAYLOADER_H_

#include <gst/gst.h>

#include "rtp/rtp_payloader.h"

namespace mediax::h264::gst::vaapi {

class RtpH264GstVaapiPayloader : public RtpPayloader {
 public:
  ///
  /// \brief Construct a new Rtp H.264 Payloader object
  ///
  ///
  RtpH264GstVaapiPayloader();

  ///
  /// \brief Destroy the Rtp H.264 Payloader object
  ///
  ///
  ~RtpH264GstVaapiPayloader() final;

  ///
  /// \brief Set the Stream Info object
  ///
  /// \param stream_information
  ///
  void SetStreamInfo(const ::mediax::StreamInformation &stream_information) override;

  ///
  /// \brief Open the RTP stream
  ///
  /// \return true
  /// \return false
  ///
  bool Open() final;

  ///
  /// \brief Close the RTP stream
  ///
  ///
  void Close() final;

  ///
  /// \brief
  ///
  /// \param rgbframe pointer to RGB buffer
  /// \param blocking set to true if blocking
  /// \return int
  ///
  int Transmit(uint8_t *rgbframe, bool blocking = true) final;

  ///
  /// \brief Stop the stream, can be quickly re-started
  ///
  ///
  void Start() final;

  ///
  /// \brief Stop the stream
  ///
  ///
  void Stop() final;

 private:
  /// Gstreamer pipeline
  GstElement *pipeline_;
  /// Pipeline started when true
  bool started_ = false;
};

}  // namespace mediax::h264::gst::vaapi

#endif  // H264_GST_VAAPI_RTP_H264_PAYLOADER_H_
