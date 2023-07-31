#include "stream_utils.h"

#include <opencv2/opencv.hpp>
using namespace cv;

void push_image(cv::Mat &frame, cv::VideoWriter &writer, int frequency = 30, int width = 640, int height = 480)
{

    if (frame.empty())
        return;

    cv::resize(frame, frame, cv::Size(width, height));

    if (frame.channels() > 3)
    {
        // If more than 3 channels, convert to 3 channels (BGR format)
        cv::cvtColor(frame, frame, cv::COLOR_RGBA2BGR); // Change conversion code accordingly if needed
    }
    else if (frame.channels() < 3)
    {
        // If less than 3 channels, do some appropriate handling
        // For example, you can duplicate the single channel to create a 3-channel image
        cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR); // Convert to BGR format
    }

    writer << frame;
    cv::waitKey(frequency);
}

int create_stream(const std::vector<std::string> &content_paths, json config)
{

    // Open Stream
    int fps = config["fps"].get<int>();
    std::string out = config["out"].get<std::string>();

    cv::VideoWriter writer;
    std::string gstreamer_pipeline = "appsrc ! videoconvert ! " + out;

    writer.open(gstreamer_pipeline, 0, (double)fps, cv::Size(config["width"].get<int>(), config["height"].get<int>()), true);
    if (!writer.isOpened())
    {
        printf("=ERR= can't create video writer\n");
        return -1;
    }

    int default_image_duration = config["image_delay"].get<int>();

    cv::Mat frame;

    // Start Stream
    int i = 0;
    std::chrono::duration<double> elapsed;
    double elapsedSeconds;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Keep track of when to find next source
    State state = Idle;
    InputFormat currentFormat = Unknown;

    int frequency = 1000 / fps;

    bool play_infinite = config["play_infinite"].get<bool>();
    ;

    cv::VideoCapture cap;

    int totalFrames;
    double video_fps;
    double videoLengthSeconds;
    double specifiedSecond;
    int positionInMillis;

    json itemConfig;
    std::string val;
    int int_val;
    int duration = 0;

    while (state != Finished)
    { // TODO: might have to add GSTREAMER specific also!
        if (state == Playing)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            elapsed = currentTime - startTime;
            elapsedSeconds = elapsed.count();

            if (elapsedSeconds >= duration)
            {
                state = Idle;
                i++;
                continue;
            }

            // perform the playing action
            if (currentFormat == Video)
            {
                cap >> frame;
            }
            else if (currentFormat == Unknown)
            {
                state = Idle;
                i++;
                continue;
            }

            push_image(frame, writer, frequency, config["width"].get<int>(), config["height"].get<int>());
        }
        else if (state == Idle)
        {
            // restart or finish loop
            if (i >= content_paths.size())
            {
                if (play_infinite)
                {
                    i = 0;
                }
                else
                {
                    state = Finished;
                    continue;
                }
            }

            // finish or play next item
            currentFormat = getInputFormat(content_paths[i]);
            std::cout << "On Item " << currentFormat << " : " << content_paths[i] << std::endl;
            startTime = std::chrono::high_resolution_clock::now();

            // Setup next item type
            if (currentFormat == Image)
            {
                frame = cv::imread(content_paths[i], cv::IMREAD_UNCHANGED);
                duration = default_image_duration;
            }
            else if (currentFormat == Video)
            {
                // remove old stream
                cap.release();

                // open new stream
                if (isJSONFile(content_paths[i]))
                {
                    itemConfig = read_json_file(content_paths[i]);
                    if (itemConfig["target"].is_number_integer())
                    {
                        int_val = itemConfig["target"].get<int>();
                        cap.open(int_val);
                    }
                    else
                    {
                        val = itemConfig["target"].get<std::string>(); // this should throw if incorrect JSON!
                        cap.open(val);
                    }
                }
                else
                {
                    val = content_paths[i];
                    cap.open(val);
                }

                if (!cap.isOpened())
                {
                    printf("=ERR= can't create video capture\n");
                    state = Idle;
                    i++;
                    continue;
                }

                // Set duration
                totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
                video_fps = cap.get(cv::CAP_PROP_FPS);
                videoLengthSeconds = static_cast<double>(totalFrames) / fps;
                duration = itemConfig.value("length", (int)videoLengthSeconds);

                // Set startpos
                specifiedSecond = itemConfig.value("start", 0);
                positionInMillis = static_cast<int>(specifiedSecond * 1000);

                // Set the position to the specified second
                cap.set(cv::CAP_PROP_POS_MSEC, positionInMillis);
            }
            else if (currentFormat == Unknown)
            {
                state = Idle;
                continue;
            }

            // continue playing
            state = Playing;
        }
    }
    cap.release();
    frame.release();
    writer.release();
    return 0;
}