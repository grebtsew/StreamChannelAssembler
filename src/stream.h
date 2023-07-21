#ifndef STREAM_H
#define STREAM_H

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <vector>


int create_stream(const std::vector<std::string>& imagePaths, int duration);

#endif // STREAM_H
