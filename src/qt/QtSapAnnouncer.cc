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
/// \file QtSapAnnouncer.cc
///

#include "qt/QtSapAnnouncer.h"

#include <iostream>
#include <memory>

namespace mediax::qt {

QtSapAnnouncer::QtSapAnnouncer(QObject *parent) : QObject(parent) {}

QtSapAnnouncer &QtSapAnnouncer::getInstance() {
  /// The singleton instance
  static QtSapAnnouncer m_instance;
  return m_instance;
}

Q_INVOKABLE void QtSapAnnouncer::addSapAnnouncement(const ::mediax::rtp::StreamInformation &stream_information) {
  sap_announcer_.AddSapAnnouncement(stream_information);
}

Q_INVOKABLE void QtSapAnnouncer::deleteSapAnnouncement(std::string_view stream_name) {
  sap_announcer_.DeleteSapAnnouncement(stream_name);
}

Q_INVOKABLE void QtSapAnnouncer::deleteAllSapAnnouncements() { sap_announcer_.DeleteAllSapAnnouncements(); }

Q_INVOKABLE void QtSapAnnouncer::start() { sap_announcer_.Start(); }

Q_INVOKABLE void QtSapAnnouncer::stop() { sap_announcer_.Stop(); }

Q_INVOKABLE void QtSapAnnouncer::setSourceInterface(uint16_t select) { sap_announcer_.SetSourceInterface(select); }

Q_INVOKABLE void QtSapAnnouncer::listInterfaces(uint16_t select) { sap_announcer_.ListInterfaces(select); }

Q_INVOKABLE uint32_t QtSapAnnouncer::getActiveStreamCount() const { return sap_announcer_.GetActiveStreamCount(); }

}  // namespace mediax::qt
