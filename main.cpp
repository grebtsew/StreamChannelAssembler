#include <iostream>
#include "src/file.h"
#include "src/stream.h"

#include <opencv2/opencv.hpp> 
using namespace cv;

int main(int argc, char* argv[]) {

    /* 1. Get config */
    std::string configJsonPath = "./Inputs/config.json"; 
    json json_object = read_json_file(configJsonPath);
    std::cout << "Config Json Collected : " << json_object << std::endl;


    /* 2. Collect all content files */
    std::string contentFolderPath = json_object["content_folder"]; 
    std::vector<std::string> allowedExtensions = {".png", ".jpg"}; 

    // Call the function to get the gathered paths
    std::vector<std::string> gatheredPaths = get_stream_content(contentFolderPath, allowedExtensions);

    // Print the gathered paths
    std::cout << "Gathered paths:" << std::endl;
    for (const std::string& path : gatheredPaths) {
        std::cout << path << std::endl;
    }

    /* 3. Create Stream Schedule TODO: */
    std::vector<std::string> playlist = gatheredPaths;

    /* 4. Start Stream */
    // Create the GStreamer pipeline
    create_stream(playlist, json_object["image_delay"].get<int>());
    }
    