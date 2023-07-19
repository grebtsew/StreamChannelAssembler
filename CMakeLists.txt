cmake_minimum_required(VERSION 3.12)
project(StreamChannelAssembler)

# Find GStreamer package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)


# Add your source files
add_executable(StreamChannelAssembler main.cpp)



# Include GStreamer headers
target_include_directories(StreamChannelAssembler PRIVATE ${GST_INCLUDE_DIRS})

# Link against GStreamer libraries
target_link_libraries(StreamChannelAssembler PRIVATE ${GST_LIBRARIES})



# TODO:
# Add source files
#add_executable(StreamChannelAssembler main.cpp)

# Add test files
#add_executable(tests test_main.cpp)

# Enable testing
#enable_testing()

# Link the tests with your project
#target_link_libraries(tests PRIVATE my_project)

# Add the tests as a test target
#add_test(NAME MyTests COMMAND tests)
