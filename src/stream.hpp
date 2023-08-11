#ifndef STREAM_HPP
#define STREAM_HPP

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>

#include "json.hpp"
#include "format.hpp"
#include "MediaProcessor.hpp"

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

#endif // STREAM_HPP
