//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
/// \brief RTP streaming video class for H.264 DEF-STAN 00-82 video streams
///
/// \file rtp_h264_payloader.h
///

#ifndef H264_GST_NVIDIA_RTP_H264_PAYLOADER_H_
#define H264_GST_NVIDIA_RTP_H264_PAYLOADER_H_

#include <gst/gst.h>

#include "rtp/rtp_payloader.h"

namespace mediax::rtp::h264::gst::nvidia {

/// A RTP payloader for H.264 DEF-STAN 00-82 video streams
class RtpH264GstNvidiaPayloader : public mediax::rtp::RtpPayloader {
 public:
  ///
  /// \brief Construct a new Rtp H.264 Payloader object
  ///
  ///
  RtpH264GstNvidiaPayloader();

  ///
  /// \brief Destroy the Rtp H.264 Payloader object
  ///
  ///
  ~RtpH264GstNvidiaPayloader() final;

  ///
  /// \brief Delete the copy constructor and copy assignment operator
  ///
  /// \param other
  ///
  RtpH264GstNvidiaPayloader(const RtpH264GstNvidiaPayloader &other) = delete;

  ///
  /// \brief Delete the copy constructor and copy assignment operator
  ///
  /// \param other
  /// \return RtpH264GstNvidiaPayloader&
  ///
  RtpH264GstNvidiaPayloader &operator=(RtpH264GstNvidiaPayloader &&other) = delete;

  ///
  /// \brief Set the Stream Info object
  ///
  /// \param stream_information
  ///
  void SetStreamInfo(const ::mediax::rtp::StreamInformation &stream_information) override;

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
  /// The Gstreamer pipeline
  GstElement *pipeline_;
};

}  // namespace mediax::rtp::h264::gst::nvidia

#endif  // H264_GST_NVIDIA_RTP_H264_PAYLOADER_H_
