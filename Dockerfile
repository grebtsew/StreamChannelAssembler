# Base image
FROM ubuntu:22.04

LABEL MAINTAINER=grebtsew UPDATED=2023-08-01

ENV DEBIAN_FRONTEND=noninteractive

# Update and install necessary packages
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    libopencv-dev \
    libgtk-3-dev \
    wget

# Build and install OpenCV from source
# Potential future problem with version changes here!
RUN git clone https://github.com/opencv/opencv.git /opt/opencv \
    && mkdir /opt/opencv/build \
    && cd /opt/opencv/build \
    && cmake .. \
    && make -j$(nproc) \
    && make install

# Set environment variables for OpenCV
ENV OPENCV_INCLUDE_DIR=/usr/local/include/opencv4
ENV OPENCV_LIB_DIR=/usr/local/lib

# Create and set the working directory
WORKDIR /app

# Copy the CMakeLists.txt and source files to the container
COPY CMakeLists.txt /app/
COPY main.cpp /app/
COPY src /app/src
COPY tests /app/tests

# Build the C++ project using CMake
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make

# Command to run the C++ project
CMD ["/app/build/StreamChannelAssembler"]