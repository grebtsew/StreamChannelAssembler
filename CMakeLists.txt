cmake_minimum_required(VERSION 3.15)
project(StreamChannelAssembler)

set(CMAKE_CXX_STANDARD 17)

# Find GStreamer package
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)


include_directories(${GST_INCLUDE_DIRS})

message("GST_INCLUDE_DIRS: ${GST_INCLUDE_DIRS}")
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message("OpenCV_LIBS: ${OpenCV_LIBS}")
message("GST_LIBRARIES: ${GST_LIBRARIES}")

# Add your source files
add_executable(StreamChannelAssembler main.cpp)

target_include_directories(StreamChannelAssembler PRIVATE ${OpenCV_INCLUDE_DIRS})

target_link_libraries(StreamChannelAssembler PRIVATE ${OpenCV_LIBS} ${GST_LIBRARIES})

# PRIVATE 
