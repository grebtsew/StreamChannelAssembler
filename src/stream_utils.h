#ifndef STREAM_UTILS_H
#define STREAM_UTILS_H

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>

#include "format.h"

enum State {
    Playing,
    Idle,
    None,
    Finished
};

int create_stream(const std::vector<std::string>& imagePaths, int duration);

#endif // STREAM_UTILS_H
