FROM ubuntu:20.04

# Install dependencies and tools
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:apt-fast/stable && \
    apt-get update && \
    apt-get -y install apt-fast && \
    apt-fast install -y libmicrohttpd-dev libjansson-dev \
    libssl-dev libsrtp-dev libsofia-sip-ua-dev libglib2.0-dev \
    libopus-dev libogg-dev libcurl4-openssl-dev liblua5.3-dev \
    libconfig-dev pkg-config gengetopt libtool automake \
    gstreamer1.0-plugins-bad # Install GStreamer plugins-bad

WORKDIR /janus
COPY . .

RUN sh autogen.sh && \
    ./configure --prefix=/opt/janus && \
    make && \
    make install && \
    make configs

CMD ["/opt/janus/bin/janus"]