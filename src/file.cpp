#include "file.hpp"
#include <filesystem>
#include <fstream>

/**
 * @brief Find all relevant content files in a provided folder.
 *
 * Detailed description of the function. This could include more information
 * about what the function does, its purpose, and any relevant details.
 *
 * @param contentFolderPath String path to content folder.
 * @param allowedExtensions Vector array of strings for each allowed file extension.
 * @return A vector array of string paths to .
 * @throws None
 *
 * @note Recursive function, that will go throught all subdirectories also.
 */
std::vector<std::string> get_stream_content(const std::string& contentFolderPath, const std::vector<std::string>& allowedExtensions)
{
    std::vector<std::string> gatheredPaths;

    for (const auto& entry : std::filesystem::directory_iterator(contentFolderPath)) {
        if (entry.is_regular_file()) {
            std::string fileExtension = entry.path().extension().string();
            for (const std::string& allowedExtension : allowedExtensions) {
                if (fileExtension == allowedExtension) {
                    gatheredPaths.push_back(entry.path().string());
                    break;
                }
            }
        } else if (entry.is_directory()) {
            // Recursive call to search for files in subdirectories
            const std::string subDirPath = entry.path().string();
            std::vector<std::string> subDirFiles = get_stream_content(subDirPath, allowedExtensions);
            gatheredPaths.insert(gatheredPaths.end(), subDirFiles.begin(), subDirFiles.end());
        }
    }

    return gatheredPaths;
}

/**
 * @brief Check if the given file has a ".json" extension.
 *
 * This function takes a file path as input and checks if the file has a ".json" extension.
 *
 * @param filePath The path of the file to check.
 * @return True if the file has a ".json" extension, false otherwise.
 */
bool isJSONFile(const std::string& filePath)
{
    // Get the last position of the dot in the file path
    size_t dotPosition = filePath.find_last_of('.');

    // Extract the file extension from the file path
    std::string fileExtension = filePath.substr(dotPosition + 1);

    // Convert the file extension to lowercase for case-insensitive comparison
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    // Check if the file extension is ".json"
    return (fileExtension == "json");
}

/**
 * @brief Read and parse a JSON file.
 *
 * This function reads the contents of the specified JSON file and parses it into a JSON object.
 *
 * @param filePath The path of the JSON file to read.
 * @return A JSON object containing the data from the JSON file.
 * @throws std::runtime_error If the file cannot be opened or read.
 */
json read_json_file(const std::string& filePath)
{
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

/**
 * @brief Get the file extension from a given file path.
 *
 * This function takes a file path as input and extracts the file extension (the part after the last dot).
 *
 * @param filePath The path of the file to get the extension from.
 * @return The file extension as a string, or an empty string if no extension is found.
 */
std::string get_file_extension(const std::string& filePath)
{
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