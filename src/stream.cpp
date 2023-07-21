// video
// https://stackoverflow.com/questions/37339184/how-to-write-opencv-mat-to-gstreamer-pipeline

#include "stream.h"

#include <opencv2/opencv.hpp> 
using namespace cv;

void push_image(cv::Mat& frame, cv::VideoWriter& writer){
        
        if (frame.empty())
            return;
            
        cv::resize(frame, frame, cv::Size(640, 480));

        if (frame.channels() > 3) {
        // If more than 3 channels, convert to 3 channels (BGR format)
        cv::cvtColor(frame, frame, cv::COLOR_RGBA2BGR); // Change conversion code accordingly if needed
    } else if (frame.channels() < 3) {
        // If less than 3 channels, do some appropriate handling
        // For example, you can duplicate the single channel to create a 3-channel image
        cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR); // Convert to BGR format
    }

        writer << frame;
        cv::waitKey( 30 );
    }

int create_stream(const std::vector<std::string>& content_paths, int duration) {

    // Open Stream
   cv::VideoWriter writer;
     writer.open("appsrc ! videoconvert ! autovideosink"
                , 0, (double)30, cv::Size(640, 480), true);
    if (!writer.isOpened()) {
        printf("=ERR= can't create video writer\n");
        return -1;
    }

    std::cout << "On Image :" << content_paths[0] << std::endl;
    cv::Mat frame = cv::imread(content_paths[0], cv::IMREAD_UNCHANGED);

    // Start Stream 
    int i = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        double elapsedSeconds = elapsed.count();
        
        
        if (elapsedSeconds >= duration) {
            i++;
            
            if (i >= content_paths.size()){
                i = 0;
            }
            frame = cv::imread(content_paths[i], cv::IMREAD_UNCHANGED);
            std::cout << "On Image :" << content_paths[i] << std::endl;
            startTime = std::chrono::high_resolution_clock::now();
        }

        push_image(frame, writer);
    }

    writer.release();
    return 0;
}