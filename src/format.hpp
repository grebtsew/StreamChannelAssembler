#ifndef FORMAT_HPP
#define FORMAT_HPP

#include <iostream>
#include <string>
#include "file.hpp"
#include <algorithm>
#include <cctype>

enum InputFormat
{
    Image,
    Video,
    Unknown
};

InputFormat getInputFormat(const std::string &filePath);
std::string toLower(const std::string &str);

#endif // FORMAT_HPP
