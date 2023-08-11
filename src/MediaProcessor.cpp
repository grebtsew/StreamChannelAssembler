// MediaProcessor.cpp
#include "MediaProcessor.hpp"

MediaProcessor::MediaProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths)
{
    config = _config;
    content_paths = _content_paths;
    width = config["width"].get<int>();
    height = config["height"].get<int>();
    fps = config["fps"].get<int>();
    frequency = 1000 / fps;
}
void MediaProcessor::process(cv::VideoWriter &writer)
{
    // TODO: fix overlay and other effects here!
    // push image and time sync with fps

    MediaProcessor::push_image(frame, writer, fps, width, height);
}
int MediaProcessor::reinitiate(int i)
{
    // Default implementation, no action required
    return 0;
}
MediaProcessor::~MediaProcessor()
{
    frame.release();
}

ImageContentProcessor::ImageContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths) : MediaProcessor(_config, _content_paths)
{
    default_image_duration = config["image_delay"].get<int>();
}
int ImageContentProcessor::reinitiate(int i)
{
    // This means to load the next image
    MediaProcessor::reinitiate(i);
    frame = cv::imread(content_paths[i], cv::IMREAD_UNCHANGED);
    duration = default_image_duration;
    return 0;
}
void ImageContentProcessor::process(cv::VideoWriter &writer)
{
    // always push the same image
    MediaProcessor::process(writer);
}
ImageContentProcessor::~ImageContentProcessor()
{
    MediaProcessor::~MediaProcessor();
}

VideoContentProcessor::VideoContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths) : MediaProcessor(_config, _content_paths)
{
}
int VideoContentProcessor::reinitiate(int i)
{
    MediaProcessor::reinitiate(i);
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
        return -1;
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
    return 0;
}
void VideoContentProcessor::process(cv::VideoWriter &writer)
{
    // read new frame from stream and send
    cap >> frame;
    MediaProcessor::process(writer);
}
VideoContentProcessor::~VideoContentProcessor()
{
    cap.release();
    MediaProcessor::~MediaProcessor();
}

GStreamerContentProcessor::GStreamerContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths) : MediaProcessor(_config, _content_paths)
{
}
int GStreamerContentProcessor::reinitiate(int i)
{
    MediaProcessor::reinitiate(i);
    return 0;
}
void GStreamerContentProcessor::process(cv::VideoWriter &writer)
{
    // read new frame from stream and send
    // cap >> frame;
    MediaProcessor::process(writer);
}
GStreamerContentProcessor::~GStreamerContentProcessor()
{
    MediaProcessor::~MediaProcessor();
}

GifContentProcessor::GifContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths) : MediaProcessor(_config, _content_paths)
{
}
int GifContentProcessor::reinitiate(int i)
{
    MediaProcessor::reinitiate(i);
    return 0;
}
void GifContentProcessor::process(cv::VideoWriter &writer)
{
    // read new frame from stream and send
    // cap >> frame;
    MediaProcessor::process(writer);
}
GifContentProcessor::~GifContentProcessor()
{
    MediaProcessor::~MediaProcessor();
}
