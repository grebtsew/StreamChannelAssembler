// MediaProcessor.cpp
#include "MediaProcessor.hpp"

class MediaProcessor
{
public:
    cv::Mat frame;
    nlohmann::json config;
    int fps;
    int height;
    int width;
    std::vector<std::string> content_paths;
    int duration;

    MediaProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths)
    {
        config = _config;
        content_paths = _content_paths;
        width = config["width"].get<int>();
        height = config["height"].get<int>();
        fps = config["fps"].get<int>();
    }
    virtual int reinitiate(int i) = 0;
    virtual void process(cv::VideoWriter &writer)
    {
        // TODO: fix overlay and other effects here!
        // push image and time sync with fps
        int frequency = 1000 / fps;
        MediaProcessor::push_image(frame, writer, fps, width, height);
    }
    virtual ~MediaProcessor() {}

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
};

class ImageContentProcessor : public MediaProcessor
{
public:
    int default_image_duration;
    ImageContentProcessor(const nlohmann::json &_config, const std::vector<std::string> &_content_paths) : MediaProcessor(_config, _content_paths)
    {
        default_image_duration = config["image_delay"].get<int>();
    }

    int reinitiate(int i) override
    {
        // This means to load the next image
        MediaProcessor::reinitiate(config);
        frame = cv::imread(content_paths[i], cv::IMREAD_UNCHANGED);
        duration = default_image_duration;
        return 0;
    }
    void process(cv::VideoWriter &writer) override
    {
        // always push the same image
        MediaProcessor::process(writer);
    }
};

class VideoContentProcessor : public MediaProcessor
{
public:
    int reinitiate(int i) override
    {
        MediaProcessor::reinitiate(config);
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
    void process(cv::VideoWriter &writer) override
    {
        // read new frame from stream and send
        cap >> frame;
        MediaProcessor::process(writer);
    }

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

class GStreamerContentProcessor : public MediaProcessor // TODO:
{
public:
    int reinitiate(int i) override
    {
        MediaProcessor::reinitiate(config);
    }
    void process(cv::VideoWriter &writer) override
    {
        // read new frame from stream and send
        // cap >> frame;
        MediaProcessor::process(writer);
    }
};

class GifContentProcessor : public MediaProcessor // TODO:
{
public:
    int reinitiate(int i) override
    {
        MediaProcessor::reinitiate(config);
    }
    void process(cv::VideoWriter &writer) override
    {
        // read new frame from stream and send
        // cap >> frame;
        MediaProcessor::process(writer);
    }
};