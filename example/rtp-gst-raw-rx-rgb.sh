#!/bin/bash

#IP_ADDRESS=238.192.1.1
IP_ADDRESS=127.0.0.1

#Gstreamer1.0 working example UYVY streaming
#===========================================

echo 'Starting example application...'
./build/bin/rtp-receive -verbose -mode=0 -ipaddr=$IP_ADDRESS -height=480 -width=640 -port=5004  &

sleep 1

echo 'Starting transmitter...'

gst-launch-1.0 videotestsrc ! video/x-raw, format=RGB, framerate=25/1, width=640, height=480 ! queue ! rtpvrawpay ! udpsink host=$IP_ADDRESS port=5004 &

sleep 10

pkill -f rtp-receive
pkill -f gst-launch-1.0
exit





