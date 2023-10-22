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
/// \file QtRtpUncompressedPayloader.h
///

#ifndef QT_QTRTPUNCOMPRESSEDPAYLOADER_H_
#define QT_QTRTPUNCOMPRESSEDPAYLOADER_H_

#include <QObject>
#include <memory>

#include "uncompressed/rtp_uncompressed_payloader.h"

namespace mediax::qt {

class QtRtpUncompressedPayloader : public QObject {
  Q_OBJECT

 public:
  ///
  /// \brief Construct a new Qt Rtp Uncompressed Payloader object
  ///
  /// \param parent
  ///
  explicit QtRtpUncompressedPayloader(QObject* parent = nullptr) : QObject(parent) {}

  ///
  /// \brief Set the Stream Info object
  ///
  /// \param stream_information
  ///
  void setStreamInfo(const mediax::rtp::StreamInformation& stream_information) {
    m_rtpPayloader.SetStreamInfo(stream_information);
  }

  ///
  /// \brief Open the RTP stream
  ///
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE bool open();

  ///
  /// \brief Start the RTP stream
  ///
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE void start();

  ///
  /// \brief Stop the RTP stream
  ///
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE void stop();

  ///
  /// \brief Close the RTP stream
  ///
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE void close();

  ///
  /// \brief Transmit a frame to the RTP stream
  ///
  /// \param frame The frame to transmit
  /// \param blocking Set to true if blocking
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE int transmit(QByteArray* frame, bool blocking = true);

 public slots:

  ///
  /// \brief A frame to transmit
  ///
  /// \param frame The frame to receive
  ///
  void sendFrame(QByteArray* frame);

 private:
  /// The underlying RTP payloader
  mediax::rtp::uncompressed::RtpUncompressedPayloader m_rtpPayloader;
};

}  // namespace mediax::qt

#endif  // QT_QTRTPUNCOMPRESSEDPAYLOADER_H_
