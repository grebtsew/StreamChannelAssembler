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
    int duration;
    MediaProcessor(const nlohmann::json &config, const std::vector<std::string> &_content_paths);
    virtual void process(cv::VideoWriter &writer) = 0;
    virtual int reinitiate(int i) = 0;
    virtual ~MediaProcessor() {}
private:
    void push_image(cv::Mat &frame, cv::VideoWriter &writer, int frequency = 30, int width = 640, int height = 480);
};

class ImageContentProcessor : public MediaProcessor
{
public:
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
};

class VideoContentProcessor : public MediaProcessor
{
public:
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
};

class GStreamerContentProcessor : public MediaProcessor
{
public:
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
};

class GifContentProcessor : public MediaProcessor
{
public:
    int reinitiate(int i) override;
    void process(cv::VideoWriter &writer) override;
};

#endif