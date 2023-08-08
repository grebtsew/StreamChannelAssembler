gst-launch-1.0 rtspsrc protocols=tcp location=rtsp://localhost:8554/test ! rtph264depay ! avdec_h264 ! autovideosink
