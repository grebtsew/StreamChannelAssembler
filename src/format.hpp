#ifndef FORMAT_HPP
#define FORMAT_HPP

#include "file.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

enum InputFormat
{
    Image,
    Video,
    Audio,
    Unknown
};

InputFormat getInputFormat(const std::string &filePath);
std::string toLower(const std::string &str);

#endif // FORMAT_HPP
