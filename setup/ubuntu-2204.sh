#!/bin/bash

#
# Installs all programs needed to execute the application locally.
#

echo "installing all"
sudo apt-get update 
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-tools \
    gstreamer1.0-rtsp \
    libopencv-dev \
    libgtk-3-dev \
    catch2 \
    ffmpeg \
    docker.io \
    docker-compose

echo "Cleaning up..."
sudo apt-get autoremove -y
sudo apt-get clean
sudo apt-get autoclean


echo "Installation and cleanup complete."
