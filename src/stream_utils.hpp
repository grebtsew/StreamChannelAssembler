#ifndef STREAM_UTILS_HPP
#define STREAM_UTILS_HPP

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>

#include "json.hpp"
#include "format.hpp"

#include <opencv2/opencv.hpp>
using namespace cv;

enum State
{
    Playing,
    Idle,
    None,
    Finished
};

int create_stream(const std::vector<std::string> &content_paths, json config);
void push_image(cv::Mat &frame, cv::VideoWriter &writer, int frequency = 30, int width = 640, int height = 480);

#endif // STREAM_UTILS_HPP
