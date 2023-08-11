#ifndef MEDIAPROCESSOR_HPP
#define MEDIAPROCESSOR_HPP

// MediaProcessor.hpp
#pragma once

#include "file.hpp"
#include "json.hpp"
#include <opencv2/opencv.hpp>

class MediaProcessor
{
public:
    cv::Mat frame;
    int duration;
    MediaProcessor(const nlohmann::json &config, const std::vector<std::string> &_content_paths);
    virtual void process(cv::VideoWriter &writer) = 0;
    virtual int reinitiate(int i) = 0;
    virtual ~MediaProcessor();

private:
    /**
     * @brief Push an image frame to a video writer.
     *
     * This function takes an image frame in the form of a cv::Mat and writes it to a cv::VideoWriter.
     * It also performs resizing and color space conversion to ensure compatibility with the video writer.
     *
     * @param frame The image frame to be pushed to the video writer.
     * @param writer The cv::VideoWriter object used for writing the frames to the video.
     * @param frequency The frequency in milliseconds to wait between pushing frames (default is 30ms).
     * @param width The desired width of the output frame (default is 640).
     * @param height The desired height of the output frame (default is 480).
     */
    void push_image(cv::Mat &frame, cv::VideoWriter &writer, int frequency, int width, int height)
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

protected:
    int fps;
    int height;
    int width;
    int frequency;
    std::vector<std::string> content_paths;
    nlohmann::json config;
};

class ImageContentProcessor : public MediaProcessor
{
public:
    ImageContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths);
    int default_image_duration;
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
    ~ImageContentProcessor() override;
};

class VideoContentProcessor : public MediaProcessor
{
public:
    VideoContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths);
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
    ~VideoContentProcessor() override;

private:
    cv::VideoCapture cap;
    json itemConfig;

    // little ugly solution to handle int/string json objects
    std::string val;
    int int_val;
    int totalFrames;
    double video_fps;
    double videoLengthSeconds;
    double specifiedSecond;
    int positionInMillis;
};

class GStreamerContentProcessor : public MediaProcessor
{
public:
    GStreamerContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths);
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
    ~GStreamerContentProcessor() override;
};

#endif