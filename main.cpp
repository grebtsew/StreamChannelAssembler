#include <iostream>
#include <opencv2/opencv.hpp>
#include <gst/gst.h>

using namespace cv;

int main() {
    // Initialize GStreamer
    gst_init(nullptr, nullptr);

    // Read an image using OpenCV
    cv::Mat image = cv::imread("./Inputs/Overlays/logo1.png");

    // Check if the image was successfully loaded
    if (image.empty()) {
        std::cout << "Failed to load image!" << std::endl;
        return 1;
    }
    std::cout << "Hello, World!" << std::endl;
     std::cout << "Hello, World! asdaiga" << std::endl;
    // Display the image using OpenCV
    cv::imshow("Image", image);
    cv::waitKey(0);

    // Cleanup and shutdown GStreamer
    gst_deinit();

    return 0;
}
