//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
///
/// \brief Session Announcement Protocol (SDP) implementation for announcement of the stream data. The SAP packet
/// contains the Session Description Protocol (SDP).
///
/// \file QtSapListener.h
///

#ifndef QT_QTSAPLISTENER_H_
#define QT_QTSAPLISTENER_H_

#include <QObject>
#include <memory>
#include <string>

#include "sap/sap_listener.h"

/// The Qt wrappers namespace
namespace mediax::qt {

class QtSapListener : public QObject {
  Q_OBJECT

 public:
  ///
  /// \brief Construct a new Qt Sap Listener object
  ///
  /// \param parent
  ///
  explicit QtSapListener(QObject *parent = nullptr);

  ///
  /// \brief Start the SAP server
  ///
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE void start();

  ///
  /// \brief Stop the SAP server
  ///
  /// \return Q_INVOKABLE
  ///
  Q_INVOKABLE void stop();

  ///
  /// \brief Get the singleton Instance object
  ///
  /// \return QtSapListener&
  ///
  static QtSapListener &getInstance();

 signals:

  ///
  /// \brief A callback for a SAP message received
  ///
  /// \param name the SAP stream session name
  /// \param message the actual message being recieved
  ///
  void sapData(const std::string name, const ::mediax::sap::SDPMessage message);

 private:
  ///
  /// \brief Process incoming SAP announcments
  ///
  /// \param sdp
  ///
  static void SapCallback(const sap::SDPMessage *sdp);

  ::mediax::sap::SAPListener sap_listener_;
};

}  // namespace mediax::qt

#endif  // QT_QTSAPLISTENER_H_