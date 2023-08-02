#ifndef FILE_HPP
#define FILE_HPP

#include "json.hpp"
#include <string>
#include <vector>

using json = nlohmann::json;

json read_json_file(const std::string& filePath);
std::vector<std::string> get_stream_content(const std::string& contentFolderPath, const std::vector<std::string>& allowedExtensions);
std::string get_file_extension(const std::string& filePath);
bool isJSONFile(const std::string& filePath);

#endif // FILE_HPP
