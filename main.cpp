#include "src/file.hpp"
#include "src/stream_utils.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

/**
 * @brief Entry point of the program.
 *
 * This is the main function of the program. It serves as the entry point and is responsible
 * for executing the core functionality of the program. The program creates gStreams that can be broadcasted from images, videos and livefeeds that are found in a folder.
 *
 * Usage: Change content in ./data/content/ folder and run the application.
 *          The application is written in C++ and built using CMake.
 *
 * @param argc The number of command-line arguments, including the program name.
 * @param argv An array of C-style strings containing the command-line arguments.
 * @return An integer status code. Typically, 0 for success and non-zero for error.
 *
 * @note None
 */
int main(int argc, char* argv[])
{
    /* 1. Get config */
    std::string configJsonPath = "./data/config.json";
    json json_object = read_json_file(configJsonPath);
    std::cout << "Config Json Collected : " << json_object << std::endl;

    /* 2. Collect all content files */
    std::string contentFolderPath = json_object["content_folder"];
    std::vector<std::string> allowedExtensions = json_object["allowed_extentions"];

    // Call the function to get the gathered paths
    std::vector<std::string>
        gatheredPaths = get_stream_content(contentFolderPath, allowedExtensions);

    // Print the gathered paths
    std::cout << "Gathered paths:" << std::endl;
    for (const std::string& path : gatheredPaths) {
        std::cout << path << std::endl;
    }

    /* 3. Create Stream Schedule */
    std::vector<std::string> playlist = gatheredPaths;

    /* 4. Start Stream */
    // Create the GStreamer pipeline
    create_stream(playlist, json_object);

    std::cout << "All Done Have a Nice Day!" << std::endl;
}
