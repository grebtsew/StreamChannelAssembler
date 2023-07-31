#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <string>
#include "file.h"
#include <algorithm>
#include <cctype>

enum InputFormat
{
    Image,
    Video,
    Unknown
};

InputFormat getInputFormat(const std::string &filePath);

#endif // FORMAT_H
