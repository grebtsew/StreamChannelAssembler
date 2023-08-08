#gst-launch-1.0 videotestsrc ! x264enc ! rtspclientsink protocols=tcp location=rtsp://localhost:8554/test
#gst-launch-1.0 videotestsrc ! videoconvert ! x264enc speed-preset=veryfast tune=zerolatency bframes=0  !   video/x-h264,profile=baseline  ! h264parse !  rtspclientsink protocols=tcp location=rtsp://127.0.0.1:8554/test
gst-launch-1.0 videotestsrc ! videoconvert ! x264enc speed-preset=ultrafast bitrate=600 key-int-max=60 ! video/x-h264,profile=baseline ! rtspclientsink protocols=tcp location=rtsp://127.0.0.1:8554/test
#(nvh264enc/nvcudah264enc för nvidia)

