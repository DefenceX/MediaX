#!/bin/bash

IP_ADDRESS=127.0.0.1

echo 'Starting example application...'
./build/bin/rtp-transmit -mode=3 -source=3 -ipaddr=$IP_ADDRESS -height=480 -width=640 -port=5004  &

sleep 1

echo 'Starting receiver...'

./build/bin/rtp-receive -mode=3 -ipaddr=$IP_ADDRESS -height=480 -width=640 -port=5004  &

sleep 10

pkill -f rtp-transmit 
pkill -f rtp-receive
exit





