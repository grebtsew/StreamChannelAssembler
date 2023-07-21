#ifndef FILE_H
#define FILE_H

#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

// Define the function to read a .json file and return the JSON object
json read_json_file(const std::string& filePath);
std::vector<std::string> get_stream_content(const std::string& contentFolderPath, const std::vector<std::string>& allowedExtensions);
std::string get_file_extension(const std::string& filePath);

#endif // FILE_H