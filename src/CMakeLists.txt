# Add the source files in the "src" directory to a library target called "get_stream_content"
add_library(file file.cpp)

# Optionally, you can include the "src" directory for header files
target_include_directories(file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

