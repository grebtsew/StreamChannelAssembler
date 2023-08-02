#include "format.h"

/**
 * @brief Convert a string to lowercase.
 *
 * This function takes a string as input and returns a new string where all characters are converted to lowercase.
 *
 * @param str The input string to be converted to lowercase.
 * @return A new string with all characters in lowercase.
 */
std::string toLower(const std::string &str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

/**
 * @brief Determine the input format based on the file extension.
 *
 * This function takes a file path as input and determines the input format based on the file extension.
 * The function compares the file extension (case-insensitive) to known image and video extensions
 * to identify the format.
 *
 * @param filePath The path of the file to determine the input format for.
 * @return An enum value representing the detected input format (Image, Video, or Unknown).
 * @see InputFormat
 */
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