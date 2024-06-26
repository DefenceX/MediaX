//
// Copyright (c) 2023, DefenceX PTY LTD
//
// This file is part of the VivoeX project developed by DefenceX.
//
// Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
// License. See the LICENSE file in the project root for full license details.
//
/// \brief Functions to convert between different colour spaces
///
/// \file colourspace_cpu.h
///

#ifndef UTILS_COLOURSPACE_CPU_H_
#define UTILS_COLOURSPACE_CPU_H_

#include <stdint.h>

#include "rtp/rtp_types.h"
#include "utils/colourspace.h"

extern "C" {
#include "libswscale/swscale.h"
}
namespace mediax::video {

/// Helper functions for different colour space options, optimised for CPU
class ColourSpaceCpu : public ColourSpace {
 public:
  ///
  /// \brief Construct a new Colour Space object
  ///
  ///
  ColourSpaceCpu() = default;

  ///
  /// \brief Destroy the Colour Space Cpu object
  ///
  ///
  ~ColourSpaceCpu() final = default;

  ///
  /// \brief Convert the image from one format to another
  ///
  /// \param res The resolution of the image
  /// \param in The input image buffer
  /// \param in_format The input image format
  /// \param in_bytes The input image bytes per pixel
  /// \param out The output image buffer
  /// \param out_format The output image format
  /// \param out_bytes The output image bytes per pixel
  /// \return int The number of bytes converted
  ///
  int Convert(mediax::rtp::Resolution res, uint8_t *in, AVPixelFormat in_format, uint8_t in_bytes, uint8_t *out,
              AVPixelFormat out_format, uint8_t out_bytes) const;

  ///
  /// \brief Convert YUV to RGBA on the CPU
  ///
  /// \param height The height of the image
  /// \param width The width of the image
  /// \param rgb The RGB image buffer
  /// \param yuv The YUV:422 image buffer
  ///
  int RgbToYuv(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *yuv) const final;

  ///
  /// \brief Convert RGB to Monochrome 8 bits ber pixel on the CPU
  ///
  /// \param height The height of the image
  /// \param width The width of the image
  /// \param rgb The RGB image buffer
  /// \param mono8 The monochrome 8 image buffer
  ///
  int RgbToMono8(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *mono8) const final;

  ///
  /// \brief Convert RGB to Monochrome 16 bits ber pixel on the CPU
  ///
  /// \param height  The height of the image
  /// \param width The width of the image
  /// \param rgb  The RGB image buffer
  /// \param mono16 The monochrome 16 image buffer
  ///
  int RgbToMono16(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *mono16) const final;

  ///
  /// \brief Convert RGBA to YUV on the CPU
  ///
  /// \param width The width of the image
  /// \param height The height of the image
  /// \param rgba The RGB image buffer
  /// \param yuv The YUV:422 image buffer
  ///
  int RgbaToYuv(uint32_t width, uint32_t height, uint8_t *rgba, uint8_t *yuv) const final;

  ///
  /// \brief Convert RGBA to RGB on the CPU
  ///
  /// \param width
  /// \param height
  /// \param rgba
  /// \param rgb
  ///
  int RgbaToRgb(uint32_t width, uint32_t height, uint8_t *rgba, uint8_t *rgb) const final;

  ///
  /// \brief Convert RGBA to BGRA on the CPU
  ///
  /// \param width
  /// \param height
  /// \param rgba
  /// \param bgra
  /// \return int
  ///
  int RgbaToBgra(uint32_t width, uint32_t height, uint8_t *rgba, uint8_t *bgra) const final;

  ///
  /// \brief Convert RGB to RGBA on the CPU
  ///
  /// \param width The height of the image
  /// \param height The width of the image
  /// \param rgb The RGB image buffer
  /// \param bgra The BGRA image buffer
  ///
  int RgbToBgra(uint32_t width, uint32_t height, uint8_t *rgb, uint8_t *bgra) const final;

  ///
  /// \brief Convert YUV to RGBA on the CPU
  ///
  /// \param height The height of the image
  /// \param width The width of the image
  /// \param yuv The YUV:422 image buffer
  /// \param bgra The BGRA image buffer
  ///
  int YuvToBgra(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *bgra) const final;

  ///
  /// \brief Convert Mono8 to RGBA on the CPU
  ///
  /// \param width  The height of the image
  /// \param height The width of the image
  /// \param mono8 The monochrome 8 bit image
  /// \param bgra The BGRA image buffer
  ///
  int Mono8ToBgra(uint32_t width, uint32_t height, uint8_t *mono8, uint8_t *bgra) const final;

  ///
  /// \brief Convert Mono16 to RGBA on the CPU
  ///
  /// \param width The height of the image
  /// \param height The width of the image
  /// \param mono16 The monochrome 16 bit image
  /// \param bgra The BGRA image buffer
  ///
  int Mono16ToBgra(uint32_t width, uint32_t height, uint8_t *mono16, uint8_t *bgra) const final;

  ///
  /// \brief Construct a new Nv12 To Bgra object
  ///
  /// \param width The height of the image
  /// \param height The width of the image
  /// \param nv12 The NV12 bit image
  /// \param bgra The BGRA image buffer
  ///
  int Nv12ToBgra(uint32_t height, uint32_t width, uint8_t *nv12, uint8_t *bgra) const;

  ///
  /// \brief Construct a new Nv12 To Rgb object
  ///
  /// \param height
  /// \param width
  /// \param nv12
  /// \param rgb
  /// \return int
  ///
  int Nv12ToRgb(uint32_t height, uint32_t width, uint8_t *nv12, uint8_t *rgb) const;

  ///
  /// \brief Convert RGB to RGBA on the CPU
  ///
  /// \param width The height of the image
  /// \param height The width of the image
  /// \param rgb The RGB image buffer
  /// \param rgba The RGBA image buffer
  ///
  int RgbToRgba(uint32_t width, uint32_t height, uint8_t *rgb, uint8_t *rgba) const final;

  ///
  /// \brief Convert YUV to RGBA on the CPU
  ///
  /// \param height The height of the image
  /// \param width The width of the image
  /// \param yuv The YUV:422 image buffer
  /// \param rgba The RGBA image buffer
  ///
  int Yuv422ToRgba(uint32_t height, uint32_t width, uint8_t *yuv422, uint8_t *rgba) const final;

  ///
  /// \brief Convert YUV420P to RGBA on the CPU
  ///
  /// \param height The height of the image
  /// \param width The width of the image
  /// \param yuv422 The YUV:420P image buffer
  /// \param rgba The RGBA image buffer
  ///
  int Yuv420ToRgba(uint32_t height, uint32_t width, uint8_t *yuv420, uint8_t *rgba) const final;

  ///
  /// \brief Convert Mono8 to RGBA on the CPU
  ///
  /// \param width  The height of the image
  /// \param height The width of the image
  /// \param mono8 The monochrome 8 bit image
  /// \param rgba The RGBA image buffer
  ///
  int Mono8ToRgba(uint32_t width, uint32_t height, uint8_t *mono8, uint8_t *rgba) const final;

  ///
  /// \brief Convert Mono8 to RGB on the CPU
  ///
  /// \param width
  /// \param height
  /// \param mono8
  /// \param rgb
  /// \return int
  ///
  int Mono8ToRgb(uint32_t width, uint32_t height, uint8_t *mono8, uint8_t *rgb) const final;

  ///
  /// \brief Convert Mono16 to RGBA on the CPU
  ///
  /// \param width The height of the image
  /// \param height The width of the image
  /// \param mono16 The monochrome 16 bit image
  /// \param rgba The RGBA image buffer
  ///
  int Mono16ToRgba(uint32_t width, uint32_t height, uint8_t *mono16, uint8_t *rgba) const final;

  ///
  /// \brief Convert Mono16 to RGBA on the CPU
  ///
  /// \param width The height of the image
  /// \param height The width of the image
  /// \param mono16 The monochrome 16 bit image
  /// \param rgb The RGB image buffer
  ///
  int Mono16ToRgb(uint32_t width, uint32_t height, uint8_t *mono16, uint8_t *rgb) const final;

  ///
  /// \brief Convert YUV to RGBA on the CPU
  ///
  /// \param height
  /// \param width
  /// \param yuv
  /// \param argb
  /// \return int
  ///
  int YuvToArgb(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *argb) const;

  ///
  /// \brief Convert YUV to RGB on the CPU
  ///
  /// \param height The height of the image
  /// \param width The width of the image
  /// \param yuv The YUV:422 image buffer
  /// \param rgb The RGB image buffer
  ///
  int YuvToRgb(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *rgb) const final;
  int Yuv420pToRgb(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *rgb) const final;

  ///
  /// \brief Scale the image to the target size.
  ///
  /// \param source_height The height of the source image
  /// \param source_width The width of the source image
  /// \param source_rgb_buffer The source image buffer
  /// \param target_height The height of the target image
  /// \param target_width The width of the target image
  /// \param target_rgb_buffer The target image buffer
  ///
  int ScaleToSizeRgb(uint32_t source_height, uint32_t source_width, uint8_t *source_rgb_buffer, uint32_t target_height,
                     uint32_t target_width, uint8_t *target_rgb_buffer) const final;

  ///
  /// \brief Scale the image to the target size.
  ///
  /// \param source_height The height of the source image
  /// \param source_width The width of the source image
  /// \param source_rgb_buffer The source image buffer
  /// \param target_height The height of the target image
  /// \param target_width The width of the target image
  /// \param target_rgba_buffer The target image buffer
  /// \return int
  ///
  int ScaleToSizeRgba(uint32_t source_height, uint32_t source_width, uint8_t *source_rgb_buffer, uint32_t target_height,
                      uint32_t target_width, uint8_t *target_rgba_buffer) const final;

  ///
  /// \brief Scale the image to the target size.
  ///
  /// \param source_height The height of the source image
  /// \param source_width The width of the source image
  /// \param source_rgb_buffer The source image buffer
  /// \param target_height The height of the target image
  /// \param target_width The width of the target image
  /// \param target_bgra_buffer The target image buffer
  /// \return int
  ///
  int ScaleToSizeBgra(uint32_t source_height, uint32_t source_width, uint8_t *source_rgb_buffer, uint32_t target_height,
                      uint32_t target_width, uint8_t *target_bgra_buffer) const;
};

}  // namespace mediax::video

#endif  // UTILS_COLOURSPACE_CPU_H_
