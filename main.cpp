#include <iostream>
#include <opencv2/opencv.hpp>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h> // Add this line for GST_APP_SRC

#include <iomanip>

#include <thread>
#include <chrono>

#include "src/file.h"

using namespace cv;


int main(int argc, char* argv[]) {
     // Initialize GStreamer
    gst_init(&argc, &argv);

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
    // Create the GStreamer pipeline
    std::string pipelineStr = "appsrc name=src ! videoconvert ! autovideosink";
    GstElement* pipeline = gst_parse_launch(pipelineStr.c_str(), nullptr);
    GstElement* appsrc = gst_bin_get_by_name(GST_BIN(pipeline), "src");

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    while (true) {
        for (const auto& imagePath : playlist) {
            // Read the image using OpenCV
            cv::Mat image = cv::imread(imagePath);
            if (image.empty()) {
                std::cerr << "Error loading image: " << imagePath << std::endl;
                continue;
            }

            // Convert OpenCV image to GStreamer buffer
            GstBuffer* buffer = gst_buffer_new_wrapped_full(
                GST_MEMORY_FLAG_READONLY, image.data, image.cols * image.rows * image.channels(), 0,
                image.cols * image.rows * image.channels(), nullptr, nullptr);

            // Set the segment format and duration in appsrc
        GstClockTime duration = json_object["image_delay"].get<int>() * GST_MSECOND;
        gst_app_src_set_caps(GST_APP_SRC(appsrc), gst_caps_new_simple("video/x-raw",
                                                                      "format", G_TYPE_STRING, "BGR",
                                                                      "width", G_TYPE_INT, image.cols,
                                                                      "height", G_TYPE_INT, image.rows,
                                                                      "framerate", GST_TYPE_FRACTION, 30, 1, nullptr));
        gst_app_src_set_duration(GST_APP_SRC(appsrc), duration);
        gst_app_src_set_size(GST_APP_SRC(appsrc), image.cols * image.rows * image.channels());

        // Push the image buffer to the pipeline
        gst_app_src_push_buffer(GST_APP_SRC(appsrc), buffer);

        // Release the memory allocated for the OpenCV image
        image.release();


            std::cout << "On Image :" << imagePath << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(json_object["image_delay"].get<int>()));

        }
    }

    // The loop is infinite, so this part is never reached. However, you can handle
    // cleanup and exit conditions here if needed.

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(appsrc);
    gst_object_unref(pipeline);

    return 0;
            
           

        
    }

    