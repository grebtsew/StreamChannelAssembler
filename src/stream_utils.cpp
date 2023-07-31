#include "stream_utils.h"

#include <opencv2/opencv.hpp> 
using namespace cv;

// fix up with this:
// https://github.com/bluenviron/mediamtx
// https://stackoverflow.com/questions/37339184/how-to-write-opencv-mat-to-gstreamer-pipeline


void push_image(cv::Mat& frame, cv::VideoWriter& writer, int frequency=30){
        
        if (frame.empty())
            return;
            
        cv::resize(frame, frame, cv::Size(640, 480));

        if (frame.channels() > 3) {
        // If more than 3 channels, convert to 3 channels (BGR format)
        cv::cvtColor(frame, frame, cv::COLOR_RGBA2BGR); // Change conversion code accordingly if needed
    } else if (frame.channels() < 3) {
        // If less than 3 channels, do some appropriate handling
        // For example, you can duplicate the single channel to create a 3-channel image
        cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR); // Convert to BGR format
    }

        writer << frame;
        cv::waitKey( frequency ); 
    }

int create_stream(const std::vector<std::string>& content_paths, json config) {

    // Open Stream
    int fps = 30; 
    cv::VideoWriter writer;
     writer.open("appsrc ! videoconvert ! autovideosink"
                , 0, (double)fps, cv::Size(640, 480), true);
    if (!writer.isOpened()) {
        printf("=ERR= can't create video writer\n");
        return -1;
    }

    int default_image_duration = config["image_delay"].get<int>();

    std::cout << "On Playlist Item :" << content_paths[0] << std::endl;
    cv::Mat frame;

    // = cv::imread(content_paths[0], cv::IMREAD_UNCHANGED);

    // Start Stream 
    int i = 0;
    std::chrono::duration<double> elapsed;
    double elapsedSeconds;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Keep track of when to find next source
    State state = Idle;
    InputFormat currentFormat = Unknown;

    int frequency = 1000 / fps;

    bool play_infinite=false;

    cv::VideoCapture cap;

    json itemConfig;
    std::string val;
    int duration = 0;
    while (state != Finished) { // TODO: might have to add GSTREAMER specific also!

        switch (state)
        {
        case Playing:

            auto currentTime = std::chrono::high_resolution_clock::now();
            elapsed = currentTime - startTime;
            elapsedSeconds = elapsed.count();

            if (elapsedSeconds >= duration) {
                        state=Idle;
                        continue;
            }

            // perform the playing action
            switch(currentFormat)
            { 
                case Image:
                break;
                case Video:
                    cap >> frame;
                break;
                default:
                // unstreamable content, move on
                state=Idle;
                continue;
                break;
            }
            
            push_image(frame, writer, frequency);
            break; // Break playing

        case Idle:

            // restart or finish loop          
            if (i >= content_paths.size()){
                if (play_infinite){
                    i = 0;
                } else {
                    state=Finished;
                    continue;
                }
            }

            // finish or play next item
            currentFormat = getInputFormat(content_paths[i]);
            
            
             //continue playing
            i++;
            state=Playing;


            // Setup next item type
            switch(currentFormat){ 
                    case Image:
                        frame = cv::imread(content_paths[i], cv::IMREAD_UNCHANGED);
                        std::cout << "On Item :" << content_paths[i] << std::endl;
                        startTime = std::chrono::high_resolution_clock::now();
                        duration = default_image_duration;
                    break;
                    case Video:
                        
                        cap.release();

                        itemConfig = read_json_file(content_paths[i]);
                        val = itemConfig["target"].get<std::string>(); // this should throw if incorrect JSON!
                        
                        cap.open(val);

                        if (!cap.isOpened()) {
                            printf("=ERR= can't create video capture\n");
                            i++;
                            state=Idle;
                        }

                        // Set duration
                        int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
                        double fps = cap.get(cv::CAP_PROP_FPS);
                        double videoLengthSeconds = static_cast<double>(totalFrames) / fps;
                        duration = itemConfig.value("length", (int)videoLengthSeconds);

                        // Set startpos
                        double specifiedSecond = itemConfig.value("start",0); 
                        int positionInMillis = static_cast<int>(specifiedSecond * 1000);

                        // Set the position to the specified second
                        cap.set(cv::CAP_PROP_POS_MSEC, positionInMillis);



                    break;
                    default:
                    state=Idle;
                    break;
                

            break; // break idle
         }

        default:
            break;
       

        
    }

    cap.release();
    frame.release();
    writer.release();
    return 0;
}