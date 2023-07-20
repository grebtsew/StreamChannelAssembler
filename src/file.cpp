#include "file.h"
#include <filesystem>
#include <fstream>

// Define the function to get stream content
std::vector<std::string> get_stream_content(const std::string& contentFolderPath, const std::vector<std::string>& allowedExtensions) {
    std::vector<std::string> gatheredPaths;

    for (const auto& entry : std::filesystem::directory_iterator(contentFolderPath)) {
        // Check if the current entry is a regular file and not a directory
        if (entry.is_regular_file()) {
            // Get the file extension of the current entry
            std::string fileExtension = entry.path().extension().string();

            // Check if the file extension is allowed
            for (const std::string& allowedExtension : allowedExtensions) {
                if (fileExtension == allowedExtension) {
                    // If allowed, add the file path to the gatheredPaths vector
                    gatheredPaths.push_back(entry.path().string());
                    break;
                }
            }
        }
    }

    return gatheredPaths;
}


json read_json_file(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    json jsonObject;

    if (inputFile.is_open()) {
        inputFile >> jsonObject;
        inputFile.close();
    } else {
        // Handle the error if the file cannot be opened or read
        throw std::runtime_error("Error: Unable to open or read the JSON file.");
    }

    return jsonObject;
}

std::string get_file_extension(const std::string& filePath) {
    // Find the last dot (.) in the file path
    size_t lastDotIndex = filePath.find_last_of(".");
    if (lastDotIndex == std::string::npos) {
        // No dot found, return an empty string (no extension)
        return "";
    }

    // Extract the substring after the dot as the file extension
    std::string extension = filePath.substr(lastDotIndex + 1);

    return extension;
}