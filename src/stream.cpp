#include "stream.hpp"

void shuffleVectorInPlace(std::vector<std::string> &inputVector)
{
    // Seed the random number generator with the current time
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 generator(static_cast<unsigned int>(seed));

    // Fisher-Yates shuffle algorithm
    for (int i = inputVector.size() - 1; i > 0; --i)
    {
        std::uniform_int_distribution<int> distribution(0, i);
        int j = distribution(generator);
        std::swap(inputVector[i], inputVector[j]);
    }
}

/**
 * @brief Create a video stream from a list of content paths using OpenCV.
 *
 * This function creates a video stream by processing a list of content paths specified in 'content_paths'
 * and a JSON configuration 'config'. The function reads image and video files, combines them into a video stream,
 * and writes the output to a file defined in the configuration.
 *
 * @param content_paths A vector of strings containing the paths of image and video files to be included in the stream.
 * @param config A JSON object containing configuration settings for the video stream.
 * @return An integer status code. 0 for success, -1 for errors.
 */
int create_stream(const std::vector<std::string> &content_paths, json config)
{
    // New abstraction
    std::shared_ptr<MediaProcessor> mediaProcessor;
    std::shared_ptr<ImageContentProcessor> image_process = std::make_shared<ImageContentProcessor>(config, content_paths);
    std::shared_ptr<VideoContentProcessor> video_process = std::make_shared<VideoContentProcessor>(config, content_paths);
    
    std::vector<std::string> updated_paths = content_paths;

    // Keep track of when to find next source
    State state = Idle;
    InputFormat currentFormat = Unknown;

    //  Setup Stream Writer
    cv::VideoWriter writer;
    int fps = config["fps"].get<int>();
    std::string out = config["out"].get<std::string>();
    std::string gstreamer_pipeline = "appsrc ! videoconvert ! " + out;
    writer.open(gstreamer_pipeline, 0, (double)fps, cv::Size(config["width"].get<int>(), config["height"].get<int>()), true);
    if (!writer.isOpened())
    {
        printf("=ERR= can't create video writer\n");
        return -1;
    }

    // additional settings
    bool play_infinite = config["play_infinite"].get<bool>();
    std::string playlist = config["playlist"].get<std::string>();
    if (playlist == "random")
    {
        shuffleVectorInPlace(updated_paths);
    }

    // initiate Time and index
    int i = 0;
    int duration = 0;
    std::chrono::duration<double> elapsed;
    double elapsedSeconds;
    auto startTime = std::chrono::high_resolution_clock::now();

    // tmp value
    int ret;
    int tmp_index;

    while (state != Finished)
    {
        if (state == Playing)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            elapsed = currentTime - startTime;
            elapsedSeconds = elapsed.count();

            // Stream next content if duration is passed
            if (elapsedSeconds >= mediaProcessor->duration || currentFormat == Unknown)
            {
                state = Idle;
                i++;
                continue;
            }

            mediaProcessor->process(writer);
        }
        else if (state == Idle)
        {
            // restart or finish loop
            if (i >= content_paths.size())
            {
                if (play_infinite)
                {
                    i = 0;
                    if (playlist == "random")
                    {
                        shuffleVectorInPlace(updated_paths);
                    }
                }
                else
                {
                    state = Finished;
                    continue;
                }
            }

            // finish or play next item
            currentFormat = getInputFormat(updated_paths[i]);

            std::cout << "On Item "
                      << " : " << updated_paths[i] << std::endl;

            startTime = std::chrono::high_resolution_clock::now();

            // Setup next item type
            if (currentFormat == Image)
            {
                mediaProcessor = image_process;
            }
            else if (currentFormat == Video)
            {
                mediaProcessor = video_process;
            }
            else if (currentFormat == Unknown)
            {
                state = Idle;
                continue;
            }

            // reload appropriate assets
            auto it = std::find(content_paths.begin(), content_paths.end(), updated_paths[i]);
            tmp_index = (it != content_paths.end()) ? std::distance(content_paths.begin(), it) : -1;
            ret = mediaProcessor->reinitiate(tmp_index);

            // Asset could not be initiated! (Alias: Stream could not be captured)
            if (ret == -1)
            {
                i++;
                continue;
            }
            // continue playing
            state = Playing;
        }
    }

    writer.release();
    return 0;
}