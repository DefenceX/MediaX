//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \brief RTP streaming video class for uncompressed DEF-STAN 00-82 video streams
///
/// \file rtp_depayloader.h
///

#ifndef RTP_RTP_DEPAYLOADER_H_
#define RTP_RTP_DEPAYLOADER_H_

#include <functional>
#include <string>
#include <vector>

#include "rtp/rtp_depayloader.h"
#include "rtp/rtp_types.h"

/// The Real Time Protocol (RTP) namespace
namespace mediax::rtp {

class RtpDepayloader;

/// \brief The RTP frame callback
using RtpCallback = std::function<void(const RtpDepayloader& depay, RtpFrameData frame)>;

/// \brief The stream state
enum class StreamState {
  /// \brief The stream is closed
  kClosed,
  /// \brief The stream is open
  kOpen,
  /// \brief The stream is started
  kStarted,
  /// \brief The stream is stopped
  kStopped
};

/// \brief The RTP stream information valid mask
enum class SettingsMask {
  hostname = 0b00000001,
  port = 0b00000010,
  name = 0b00000100,
  height = 0b00001000,
  width = 0b00010000,
  framerate = 0b00100000,
  encoding = 0b01000000,
  all = 0b01111111
};

///
/// \brief Manage an RTP stream
///
///
class RtpDepayloader {
 public:
  ///
  /// \brief Construct a new Rtp Depayloader object
  ///
  ///
  RtpDepayloader();

  ///
  /// \brief Destroy the Rtp Stream object
  ///
  ///
  virtual ~RtpDepayloader() = default;

  ///
  /// \brief Construct a new Rtp Payloader object
  ///
  ///
  RtpDepayloader(RtpDepayloader const&) = delete;

  ///
  /// \brief Construct a new Rtp Payloader object
  ///
  /// \return RtpPayloader&
  ///
  RtpDepayloader& operator=(RtpDepayloader const&) = delete;

  ///
  /// \brief Configure at RTP input stream and dont wait for the SAP/SDP announcement
  ///
  /// \param stream_information set the stream information
  ///
  virtual void SetStreamInfo(const ::mediax::rtp::StreamInformation& stream_information) = 0;

  ///
  /// \brief Register a callback for our session_name
  ///
  /// \param callback The callback to notify when a frame is received
  ///
  void RegisterCallback(const ::mediax::rtp::RtpCallback& callback);

  ///
  /// \brief Open the RTP stream
  ///
  /// \return true
  /// \return false
  ///
  virtual bool Open() {
    state_ = StreamState::kOpen;
    return true;
  }

  ///
  /// \brief Start the stream
  ///
  ///
  virtual void Start() { state_ = StreamState::kStarted; }

  ///
  /// \brief Stop the stream, can be quickly re-started
  ///
  ///
  virtual void Stop() { state_ = StreamState::kStopped; }

  ///
  /// \brief Close the RTP stream
  ///
  ///
  virtual void Close() { state_ = StreamState::kClosed; }

  ///
  /// \brief Recieve a frame or timeout
  ///
  /// \param data the fame buffer in CPU memory.
  /// \param timeout zero will wait forever or a timeout in milliseconds
  /// \return true when frame available
  /// \return false when no frame was received in the timeout
  ///
  virtual bool Receive(mediax::rtp::RtpFrameData* data, int32_t timeout = 0) = 0;

  ///
  /// \brief Set the Session Name attribute
  ///
  /// \param name The SAP/SDP session name
  ///
  void SetSessionName(std::string_view name);

  ///
  /// \brief Get the Session Name attribute
  ///
  /// \return std::string The SAP/SDP session name
  ///
  std::string GetSessionName();

  ///
  /// \brief Get the Colour Space object of the incoming stream. \note This may be invalid id no SAP/SDP announcement
  /// has been received yet.
  ///
  /// \return ColourspaceType
  ///
  ::mediax::rtp::ColourspaceType GetColourSpace();

  ///
  /// \brief Set the Height attribute
  ///
  ///
  void SetHeight(uint32_t height);

  ///
  /// \brief Get the Height object of the incoming stream. \note This may be invalid id no SAP/SDP announcement has been
  /// received yet.
  ///
  /// \return uint32_t
  ///
  uint32_t GetHeight() const;

  ///
  /// \brief Set the Width attribute
  ///
  ///
  void SetWidth(uint32_t width);

  ///
  /// \brief Get the Width object of the incoming stream. \note This may be invalid id no SAP/SDP announcement has been
  /// received yet.
  ///
  /// \return uint32_t
  ///
  uint32_t GetWidth() const;

  ///
  /// \brief Set the Frame Rate object
  ///
  ///
  void SetFramerate(uint32_t framerate);

  ///
  /// \brief Get the Frame Rate of the incoming stream. \note This may be invalid id no SAP/SDP announcement has been
  /// received yet.
  ///
  /// \return uint32_t
  ///
  uint32_t GetFrameRate() const;

  ///
  /// \brief Get the Ip Address of the incoming stream. \note This may be invalid id no SAP/SDP announcement has been
  /// received yet.
  ///
  /// \return std::string
  ///
  std::string GetIpAddress() const;

  ///
  /// \brief Set the Ip Address attribute
  ///
  /// \param ip_address The IPV4 address of the video stream
  ///
  void SetIpAddress(std::string_view ip_address);

  ///
  /// \brief Set the Port object
  ///
  ///
  void SetPort(uint32_t port);

  ///
  /// \brief Get the Port of the incoming stream. \note This may be invalid id no SAP/SDP announcement has been
  /// received yet.
  ///
  /// \return uint32_t
  ///
  uint32_t GetPort() const;

  ///
  /// \brief Set the Colour Space object
  ///
  /// \param colourspace
  ///
  void SetColourSpace(::mediax::rtp::ColourspaceType colourspace);

  ///
  /// \brief Check if the settings are valid. All stream information was set
  ///
  /// \return true
  /// \return false
  ///
  bool SettingsValid() const;

  ///
  /// \brief Get the Colour Space object
  ///
  /// \return ::mediax::rtp::ColourspaceType
  ///
  ::mediax::rtp::ColourspaceType GetColourSpace() const;

  ///
  /// \brief Check if the IP address is a multicast address
  ///
  /// \param ip_address
  /// \return true
  /// \return false
  ///
  bool IsMulticast(std::string_view ip_address) const;

  ///
  /// \brief Check is a callback is registered
  ///
  /// \return true if one is registered
  /// \return false if one is not registered
  ///
  bool CallbackRegistered() const;

  ///
  /// \brief Unregister the callback
  ///
  ///
  void UnregisterCallback();

  ///
  /// \brief Call the registered callback
  ///
  /// \param frame the callback frame data
  ///
  virtual void Callback(::mediax::rtp::RtpFrameData frame) const = 0;

  ///
  /// \brief Get the Port Type object
  ///
  /// \return ::mediax::rtp::RtpPortType&
  ///
  ::mediax::rtp::RtpPortType& GetStream();

  ///
  /// \brief Get the Callback object
  ///
  /// \return ::mediax::rtp::RtpCallback
  ///
  ::mediax::rtp::RtpCallback GetCallback() const { return callback_; }

  ///
  /// \brief Get the Buffer object
  ///
  /// \return std::vector<uint8_t>&
  ///
  std::vector<uint8_t>& GetBuffer() { return buffer_in_; }

  ///
  /// \brief Get the Buffer Size object
  ///
  /// \return uint32_t
  ///
  uint32_t GetBufferSize() const { return static_cast<uint32_t>(buffer_in_.size()); }

  ///
  /// \brief Set the Buffer Size object
  ///
  /// \param size
  ///
  void SetBufferSize(uint32_t size) { buffer_in_.resize(size); }

  ///
  /// \brief Get the State object
  ///
  /// \return ::mediax::rtp::StreamState
  ///
  ::mediax::rtp::StreamState GetState() const { return state_; }

 private:
  /// Stream state
  ::mediax::rtp::StreamState state_ = ::mediax::rtp::StreamState::kClosed;
  /// The callback to notify when a frame is received
  ::mediax::rtp::RtpCallback callback_;
  /// UDP data buffer
  std::vector<uint8_t> buffer_in_;
  /// Indicate if callback has been registered
  bool callback_registered_ = false;
  /// Ingress port
  ::mediax::rtp::RtpPortType ingress_ = {};
  /// Settings valid mask
  uint32_t settings_mask_ = 0;
};  // namespace mediax::rtp

}  // namespace mediax::rtp

#endif  // RTP_RTP_DEPAYLOADER_H_
