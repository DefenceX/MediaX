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
/// \file colourspace_cpu.cc
///

#include "utils/colourspace_cpu.h"

#include <array>
#include <memory>
#include <vector>

#include "rtp/rtp.h"

extern "C" {
#include "libswscale/swscale.h"
}

namespace mediax::video {

int ColourSpaceCpu::Convert(mediax::rtp::Resolution res, uint8_t *in, AVPixelFormat in_format, uint8_t in_bytes,
                            uint8_t *out, AVPixelFormat out_format, uint8_t out_bytes) const {
  if (!in || !out) {
    // Handle null pointers gracefully
    return 1;
  }

  std::unique_ptr<SwsContext, decltype(&sws_freeContext)> ctx(
      sws_getContext(res.width, res.height, in_format, res.width, res.height, out_format, SWS_BICUBIC, nullptr, nullptr,
                     nullptr),
      &sws_freeContext);
  if (!ctx) {
    // Handle allocation failure gracefully
    return 1;
  }

  const std::array<uint8_t *, 1> inData = {in};
  std::array<uint8_t *, 1> outData = {out};

  // Use static_cast instead of C-style cast
  const std::array<int32_t, 1> inLinesize = {(int32_t)(res.width * in_bytes)};
  std::array<int32_t, 1> outLinesize = {(int32_t)(res.width * out_bytes)};

  sws_scale(ctx.get(), inData.data(), inLinesize.data(), 0, res.height, outData.data(), outLinesize.data());
  return 0;
}

int ColourSpaceCpu::YuvToRgb(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *rgb) const {
  return Convert({width, height}, yuv, AV_PIX_FMT_UYVY422, 2, rgb, AV_PIX_FMT_RGB24, 3);
}

// int ColourSpaceCpu::Yuv420pToRgb(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *rgb) const {
//   return Convert({width, height}, yuv, AV_PIX_FMT_YUV420P, 3, rgb, AV_PIX_FMT_RGB24, 3);
// }

int ColourSpaceCpu::Yuv420pToRgb(uint32_t height, uint32_t width, uint8_t *yuv420p, uint8_t *rgb) const {
  if (!yuv420p || !rgb) {
    return -1;  // Invalid input pointers
  }

  uint32_t frameSize = width * height;
  uint32_t chromaSize = frameSize / 4;
  uint8_t *yPlane = yuv420p;
  uint8_t *uPlane = yuv420p + frameSize;
  uint8_t *vPlane = yuv420p + frameSize + chromaSize;

  for (uint32_t j = 0; j < height; ++j) {
    for (uint32_t i = 0; i < width; ++i) {
      uint32_t yIndex = j * width + i;
      uint32_t uvIndex = (j / 2) * (width / 2) + (i / 2);

      uint8_t Y = yPlane[yIndex];
      uint8_t U = uPlane[uvIndex];
      uint8_t V = vPlane[uvIndex];

      int C = Y - 16;
      int D = U - 128;
      int E = V - 128;

      int R = (298 * C + 409 * E + 128) >> 8;
      int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
      int B = (298 * C + 516 * D + 128) >> 8;

      R = R < 0 ? 0 : (R > 255 ? 255 : R);
      G = G < 0 ? 0 : (G > 255 ? 255 : G);
      B = B < 0 ? 0 : (B > 255 ? 255 : B);

      uint32_t rgbIndex = yIndex * 3;
      rgb[rgbIndex] = static_cast<uint8_t>(R);
      rgb[rgbIndex + 1] = static_cast<uint8_t>(G);
      rgb[rgbIndex + 2] = static_cast<uint8_t>(B);
    }
  }

  return 0;  // Success
}

int ColourSpaceCpu::YuvToBgra(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *gbra) const {
  return Convert({width, height}, yuv, AV_PIX_FMT_UYVY422, 2, gbra, AV_PIX_FMT_BGRA, 4);
}

int ColourSpaceCpu::RgbaToRgb(uint32_t width, uint32_t height, uint8_t *rgba, uint8_t *rgb) const {
  return Convert({width, height}, rgba, AV_PIX_FMT_RGBA, 4, rgb, AV_PIX_FMT_RGB24, 3);
}

int ColourSpaceCpu::RgbaToBgra(uint32_t width, uint32_t height, uint8_t *rgba, uint8_t *rgb) const {
  return Convert({width, height}, rgba, AV_PIX_FMT_RGBA, 4, rgb, AV_PIX_FMT_BGRA, 4);
}

int ColourSpaceCpu::RgbaToYuv(uint32_t height, uint32_t width, uint8_t *rgba, uint8_t *yuv) const {
  return Convert({width, height}, rgba, AV_PIX_FMT_RGBA, 4, yuv, AV_PIX_FMT_UYVY422, 2);
}

int ColourSpaceCpu::RgbToYuv(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *yuv) const {
  return Convert({width, height}, rgb, AV_PIX_FMT_RGB24, 4, yuv, AV_PIX_FMT_UYVY422, 2);
}

int ColourSpaceCpu::RgbToMono8(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *mono8) const {
  return Convert({width, height}, rgb, AV_PIX_FMT_RGB24, 3, mono8, AV_PIX_FMT_GRAY8, 1);
}

int ColourSpaceCpu::RgbToMono16(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *mono16) const {
  return Convert({width, height}, rgb, AV_PIX_FMT_RGB24, 3, mono16, AV_PIX_FMT_GRAY16, 2);
}

int ColourSpaceCpu::RgbToBgra(uint32_t height, uint32_t width, uint8_t *rgb, uint8_t *bgra) const {
  return Convert({width, height}, rgb, AV_PIX_FMT_RGB24, 3, bgra, AV_PIX_FMT_BGRA, 4);
}

int ColourSpaceCpu::Mono8ToBgra(uint32_t width, uint32_t height, uint8_t *mono8, uint8_t *bgra) const {
  return Convert({width, height}, mono8, AV_PIX_FMT_GRAY8, 1, bgra, AV_PIX_FMT_BGRA, 4);
}

int ColourSpaceCpu::Mono16ToBgra(uint32_t width, uint32_t height, uint8_t *mono16, uint8_t *bgra) const {
  return Convert({width, height}, mono16, AV_PIX_FMT_GRAY16, 2, bgra, AV_PIX_FMT_BGRA, 4);
}

int ColourSpaceCpu::Nv12ToBgra(uint32_t height, uint32_t width, uint8_t *nv12, uint8_t *bgra) const {
  if (!bgra || !nv12) {
    // Handle null pointers gracefully
    return 1;
  }

  std::unique_ptr<SwsContext, decltype(&sws_freeContext)> ctx(
      sws_getContext(width, height, AV_PIX_FMT_NV12, width, height, AV_PIX_FMT_BGRA, SWS_BICUBIC, nullptr, nullptr,
                     nullptr),
      &sws_freeContext);
  if (!ctx) {
    // Handle allocation failure gracefully
    return 1;
  }

  const std::array<uint8_t *, 2> inData = {nv12, nv12 + width * height};
  std::array<uint8_t *, 1> outData = {bgra};

  // Use static_cast instead of C-style cast
  const std::array<int32_t, 2> inLinesize = {(int32_t)(width), (int32_t)(width)};
  std::array<int32_t, 1> outLinesize = {(int32_t)(width * 4)};

  sws_scale(ctx.get(), inData.data(), inLinesize.data(), 0, height, outData.data(), outLinesize.data());

  return 0;
}

int ColourSpaceCpu::Nv12ToRgb(uint32_t height, uint32_t width, uint8_t *nv12, uint8_t *rgb) const {
  if (!rgb || !nv12) {
    // Handle null pointers gracefully
    return 1;
  }

  std::unique_ptr<SwsContext, decltype(&sws_freeContext)> ctx(
      sws_getContext(width, height, AV_PIX_FMT_NV12, width, height, AV_PIX_FMT_RGB24, SWS_BICUBIC, nullptr, nullptr,
                     nullptr),
      &sws_freeContext);
  if (!ctx) {
    // Handle allocation failure gracefully
    return 1;
  }

  const std::array<uint8_t *, 2> inData = {nv12, nv12 + width * height};
  std::array<uint8_t *, 1> outData = {rgb};

  // Use static_cast instead of C-style cast
  const std::array<int32_t, 2> inLinesize = {(int32_t)(width), (int32_t)(width)};
  std::array<int32_t, 1> outLinesize = {(int32_t)(width * 3)};

  sws_scale(ctx.get(), inData.data(), inLinesize.data(), 0, height, outData.data(), outLinesize.data());

  return 0;
}

int ColourSpaceCpu::YuvToArgb(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *argb) const {
  return Convert({width, height}, yuv, AV_PIX_FMT_UYVY422, 2, argb, AV_PIX_FMT_ARGB, 4);
}

int ColourSpaceCpu::RgbToRgba(uint32_t width, uint32_t height, uint8_t *rgb, uint8_t *rgba) const {
  return Convert({width, height}, rgb, AV_PIX_FMT_RGB24, 3, rgba, AV_PIX_FMT_RGBA, 4);
}

int ColourSpaceCpu::Yuv422ToRgba(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *rgba) const {
  return Convert({width, height}, yuv, AV_PIX_FMT_UYVY422, 2, rgba, AV_PIX_FMT_RGBA, 4);
}

int ColourSpaceCpu::Yuv420ToRgba(uint32_t height, uint32_t width, uint8_t *yuv, uint8_t *rgba) const {
  return Convert({width, height}, yuv, AV_PIX_FMT_YUV420P, 3, rgba, AV_PIX_FMT_RGBA, 4);
}

int ColourSpaceCpu::Mono8ToRgba(uint32_t width, uint32_t height, uint8_t *mono8, uint8_t *rgba) const {
  return Convert({width, height}, mono8, AV_PIX_FMT_GRAY8, 1, rgba, AV_PIX_FMT_RGBA, 4);
}

int ColourSpaceCpu::Mono8ToRgb(uint32_t width, uint32_t height, uint8_t *mono8, uint8_t *rgb) const {
  return Convert({width, height}, mono8, AV_PIX_FMT_GRAY8, 1, rgb, AV_PIX_FMT_RGB24, 3);
}

int ColourSpaceCpu::Mono16ToRgba(uint32_t width, uint32_t height, uint8_t *mono16, uint8_t *rgba) const {
  return Convert({width, height}, mono16, AV_PIX_FMT_GRAY16, 2, rgba, AV_PIX_FMT_RGBA, 4);
}

int ColourSpaceCpu::Mono16ToRgb(uint32_t width, uint32_t height, uint8_t *mono16, uint8_t *rgb) const {
  return Convert({width, height}, mono16, AV_PIX_FMT_GRAY16, 2, rgb, AV_PIX_FMT_RGB24, 3);
}

int ColourSpaceCpu::ScaleToSizeRgb(uint32_t source_height, uint32_t source_width, uint8_t *source_rgb_buffer,
                                   uint32_t target_height, uint32_t target_width, uint8_t *target_rgb_buffer) const {
  if (!source_rgb_buffer || !target_rgb_buffer) {
    // Handle null pointers gracefully
    return 1;
  }

  std::unique_ptr<SwsContext, decltype(&sws_freeContext)> ctx(
      sws_getContext(source_width, source_height, AV_PIX_FMT_RGB24, target_width, target_height, AV_PIX_FMT_RGB24,
                     SWS_BICUBIC, nullptr, nullptr, nullptr),
      &sws_freeContext);
  if (!ctx) {
    // Handle allocation failure gracefully
    return 2;
  }

  const std::array<uint8_t *, 1> inData = {source_rgb_buffer};
  std::array<uint8_t *, 1> outData = {target_rgb_buffer};

  // Use static_cast instead of C-style cast
  const std::array<int32_t, 1> inLinesize = {(int32_t)(source_width * 3)};
  std::array<int32_t, 1> outLinesize = {(int32_t)(target_width * 3)};
  sws_scale(ctx.get(), inData.data(), inLinesize.data(), 0, source_height, outData.data(), outLinesize.data());

  return 0;
}

int ColourSpaceCpu::ScaleToSizeRgba(uint32_t source_height, uint32_t source_width, uint8_t *source_rgb_buffer,
                                    uint32_t target_height, uint32_t target_width, uint8_t *target_rgba_buffer) const {
  if (!source_rgb_buffer || !target_rgba_buffer) {
    // Handle null pointers gracefully
    return 1;
  }

  std::unique_ptr<SwsContext, decltype(&sws_freeContext)> ctx(
      sws_getContext(source_width, source_height, AV_PIX_FMT_RGBA, target_width, target_height, AV_PIX_FMT_RGBA,
                     SWS_BICUBIC, nullptr, nullptr, nullptr),
      &sws_freeContext);
  if (!ctx) {
    // Handle allocation failure gracefully
    return 2;
  }

  const std::array<uint8_t *, 1> inData = {source_rgb_buffer};
  std::array<uint8_t *, 1> outData = {target_rgba_buffer};

  // Use static_cast instead of C-style cast
  const std::array<int32_t, 1> inLinesize = {(int32_t)(source_width * 4)};
  std::array<int32_t, 1> outLinesize = {(int32_t)(target_width * 4)};
  sws_scale(ctx.get(), inData.data(), inLinesize.data(), 0, source_height, outData.data(), outLinesize.data());

  return 0;
}

int ColourSpaceCpu::ScaleToSizeBgra(uint32_t source_height, uint32_t source_width, uint8_t *source_rgb_buffer,
                                    uint32_t target_height, uint32_t target_width, uint8_t *target_bgra_buffer) const {
  if (!source_rgb_buffer || !target_bgra_buffer) {
    // Handle null pointers gracefully
    return 1;
  }

  std::unique_ptr<SwsContext, decltype(&sws_freeContext)> ctx(
      sws_getContext(source_width, source_height, AV_PIX_FMT_BGRA, target_width, target_height, AV_PIX_FMT_BGRA,
                     SWS_BICUBIC, nullptr, nullptr, nullptr),
      &sws_freeContext);
  if (!ctx) {
    // Handle allocation failure gracefully
    return 2;
  }

  const std::array<uint8_t *, 1> inData = {source_rgb_buffer};
  std::array<uint8_t *, 1> outData = {target_bgra_buffer};

  // Use static_cast instead of C-style cast
  const std::array<int32_t, 1> inLinesize = {(int32_t)(source_width * 4)};
  std::array<int32_t, 1> outLinesize = {(int32_t)(target_width * 4)};
  sws_scale(ctx.get(), inData.data(), inLinesize.data(), 0, source_height, outData.data(), outLinesize.data());

  return 0;
}

}  // namespace mediax::video
