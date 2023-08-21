#!/bin/bash

docker-compose up -d --build

echo "Will wait 10 seconds and then try to connect"
sleep 10

if command -v vlc &> /dev/null; then
    echo "VLC is installed. Using VLC to play the video."
    vlc rtsp://localhost:8554/livestream
elif command -v gst-launch-1.0 &> /dev/null; then
    echo "VLC is not installed. Using GStreamer to play the video."
    gst-launch-1.0 playbin uri= rtsp://localhost:8554/livestream
else
    echo "Neither VLC nor GStreamer is installed. Cannot play the video."
fi
