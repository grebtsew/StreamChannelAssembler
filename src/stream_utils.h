#ifndef STREAM_UTILS_H
#define STREAM_UTILS_H

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>

#include "json.hpp"
#include "format.h"

enum State
{
    Playing,
    Idle,
    None,
    Finished
};

int create_stream(const std::vector<std::string> &content_paths, json config);

#endif // STREAM_UTILS_H
