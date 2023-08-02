cmake_minimum_required(VERSION 3.15)
project(StreamChannelAssembler)

set(CMAKE_CXX_STANDARD 17)

# Find GStreamer package
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Catch2 REQUIRED)
pkg_check_modules(gtk3 REQUIRED IMPORTED_TARGET gtk+-3.0)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0>=1.4)
pkg_search_module(gstreamer-sdp REQUIRED IMPORTED_TARGET gstreamer-sdp-1.0>=1.4)
pkg_search_module(gstreamer-app REQUIRED IMPORTED_TARGET gstreamer-app-1.0>=1.4)
pkg_search_module(gstreamer-video REQUIRED IMPORTED_TARGET gstreamer-video-1.0>=1.4)


include_directories(${GST_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

message("GST_INCLUDE_DIRS: ${GST_INCLUDE_DIRS}")
message("OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message("OpenCV_LIBS: ${OpenCV_LIBS}")
message("GST_LIBRARIES: ${GST_LIBRARIES}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")


# Add your source files
add_executable(StreamChannelAssembler main.cpp src/format.cpp src/file.cpp src/stream_utils.cpp)

add_subdirectory(src)

target_include_directories(StreamChannelAssembler PRIVATE ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(StreamChannelAssembler PRIVATE ${OpenCV_LIBS} ${GST_LIBRARIES} file format stream_utils
    PkgConfig::gtk3
    PkgConfig::gstreamer
    PkgConfig::gstreamer-sdp
    PkgConfig::gstreamer-app
    PkgConfig::gstreamer-video)


# Add your test files and create the test executable
add_executable(tests tests/test_main.cpp tests/test_to_lower.cpp tests/test_get_input_format.cpp tests/test_create_stream.cpp tests/test_push_image.cpp src/format.cpp src/file.cpp src/stream_utils.cpp)

# Link the tests with the Catch2 library (or your preferred testing library)
target_link_libraries(tests PRIVATE Catch2::Catch2 ${OpenCV_LIBS} file format stream_utils)
