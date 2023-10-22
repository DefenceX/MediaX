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
/// \file QtRtpUncompressedDepayloader.cc
///

#include "qt/QtRtpUncompressedDepayloader.h"

namespace mediax::qt {
bool QtRtpUncompressedDepayloader::receive(uint8_t** cpu, int32_t timeout) {
  return m_depayloader->Receive(cpu, timeout);
}

Q_INVOKABLE mediax::rtp::StreamState QtRtpUncompressedDepayloader::getState() { return m_depayloader->GetState(); }

}  // namespace mediax::qt
