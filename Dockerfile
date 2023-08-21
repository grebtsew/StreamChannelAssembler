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
    gstreamer1.0-tools \
    gstreamer1.0-rtsp \
    libopencv-dev \
    libgtk-3-dev \
    catch2 \
    ffmpeg \
    dos2unix  


# Create and set the working directory
WORKDIR /app

# Copy the CMakeLists.txt and source files to the container
COPY CMakeLists.txt /app/
COPY src /app/src
COPY tests /app/tests
COPY docker /docker
COPY servers /servers

RUN dos2unix /docker/entrypoint.sh
RUN chmod +x /docker/entrypoint.sh

# Command to run the C++ project
CMD ["/docker/entrypoint.sh"]