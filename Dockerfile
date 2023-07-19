# Base image
FROM ubuntu:21.04

# Update and install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Build the application
RUN cmake .
RUN make

# Set the entrypoint
CMD ["./your_application"]