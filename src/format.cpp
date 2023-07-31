#include "format.h"

std::string toLower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

InputFormat getInputFormat(const std::string &filePath)
{

    // Find the position of the last dot in the file path
    size_t dotPos = filePath.find_last_of('.');

    if (dotPos != std::string::npos)
    {
        // Extract the file extension from the file path
        std::string extension = filePath.substr(dotPos + 1);

        // Convert the extension to lowercase for case-insensitive comparison
        extension = toLower(extension);

        // Check the extension and return the appropriate file type
        if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif")
        {
            return Image;
        }
        else if (extension == "json")
        {
            // json json_object = read_json_file(filePath);
            return Video;
        }
        else if (extension == "mp4" || extension == "avi" || extension == "mov")
        {
            return Video;
        }
    }

    return Unknown;
}