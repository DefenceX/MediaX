%module rtp_h264_gst_vaapi_payloader

%import "rtp/rtp_payloader.i"

%{
#include "h264/gst/vaapi/rtp_h264_payloader.h"
%}

%include "h264/gst/vaapi/rtp_h264_payloader.h" 