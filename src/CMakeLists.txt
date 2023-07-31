# Add the source files in the "src" directory to a library target called "get_stream_content"
add_library(file file.cpp)
add_library(stream stream.cpp)
add_library(stream_utils stream_utils.cpp)
add_library(format format.cpp)

# Optionally, you can include the "src" directory for header files
target_include_directories(file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(stream PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(stream_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(format PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})