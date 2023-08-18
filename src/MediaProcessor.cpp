// MediaProcessor.cpp
#include "MediaProcessor.hpp"

MediaProcessor::MediaProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths)
{
    config = _config;
    content_paths = _content_paths;
    width = config["width"].get<int>();
    height = config["height"].get<int>();
    fps = config["fps"].get<int>();

    // prepare overlays
    std::vector<std::string> streamOverlay = config["stream_overlays"];
    for (const std::string &overlay : streamOverlay)
    {
        json json_object = read_json_file(overlay);
        cv::Mat overlayImage = cv::imread(json_object["image"].get<std::string>());

        alpha = json_object["alpha"].get<double>();
        x = json_object["x"].get<int>();
        y = json_object["y"].get<int>();

        // Resize
        cv::resize(overlayImage, overlayImage, cv::Size(json_object["width"].get<int>(), json_object["height"].get<int>()));
        // Push
        overlayImages.push_back(overlayImage);
    }

    frequency = 1000 / fps;
}
void MediaProcessor::process(cv::VideoWriter &writer)
{
    if (frame.empty())
        return;

    // resize image
    cv::resize(frame, frame, cv::Size(width, height));

    // fixes .png errors
    convertToBGR(frame);

    // add overlays on images
    MediaProcessor::performOverlays();

    // push image and time sync with fps
    MediaProcessor::push_image(frame, writer, fps, width, height);
}

void convertToBGR(cv::Mat &image)
{
    if (image.channels() > 3)
    {
        cv::cvtColor(image, image, cv::COLOR_RGBA2BGR); // Change conversion code accordingly if needed
    }
    else if (image.channels() < 3)
    {
        cv::cvtColor(image, image, cv::COLOR_GRAY2BGR); // Convert to BGR format
    }
}

void MediaProcessor::performOverlays()
{
    // Perform Overlay of Logos
    for (size_t i = 0; i < overlayImages.size(); ++i)
    {
        cv::Mat &image = overlayImages[i];
        cv::Rect roi(x, y, image.cols, image.rows);

        cv::addWeighted(frame(roi), 1.0 - alpha, image(cv::Rect(0, 0, roi.width, roi.height)), alpha, 0, frame(roi));
    }
}

int MediaProcessor::reinitiate(int i)
{
    // Default implementation, no action required
    return 0;
}
MediaProcessor::~MediaProcessor()
{
    frame.release();
    overlayImages.clear();
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

    if (frame.empty())
        return -1;

    duration = default_image_duration;
    cv::resize(frame, frame, cv::Size(width, height));
    convertToBGR(frame);
    MediaProcessor::performOverlays();

    return 0;
}
void ImageContentProcessor::process(cv::VideoWriter &writer)
{

    // push image and time sync with fps
    MediaProcessor::push_image(frame, writer, fps, width, height);
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
            if (itemConfig.contains("type")){
                if (itemConfig["type"] == "gstreamer"){
                    cap.open(val,cv::CAP_GSTREAMER);
                } else if (itemConfig["type"] == "ffmpeg") {
                    cap.open(val,cv::CAP_FFMPEG);
                } else {
                    cap.open(val);
                }
            } else {
                cap.open(val);
            }
            
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

    if (isJSONFile(content_paths[i]))
    {
        duration = itemConfig.value("length", (int)videoLengthSeconds);

        // Set startpos
        specifiedSecond = itemConfig.value("start", 0);
        positionInMillis = static_cast<int>(specifiedSecond * 1000);

        // Set the position to the specified second
        cap.set(cv::CAP_PROP_POS_MSEC, positionInMillis);
    }
    else
    {
        duration = (int)videoLengthSeconds;
    }

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
