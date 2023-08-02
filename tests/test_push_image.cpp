#include <catch2/catch.hpp>
#include <opencv2/opencv.hpp>
#include "../src/stream_utils.hpp" // Include the header containing the 'push_image' function

TEST_CASE("Test 'push_image' function", "[push_image]")
{
    cv::Mat frame = cv::imread("../data/contents/examples/test/4.jpg", cv::IMREAD_COLOR);
    cv::VideoWriter writer("output_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, frame.size());

    // Test the function without resizing the frame
    push_image(frame, writer, 100);

    // Test the function with resizing the frame
    push_image(frame, writer, 100, 640, 480);

    // Test the function with an empty frame (should not throw)
    cv::Mat emptyFrame;
    REQUIRE_NOTHROW(push_image(emptyFrame, writer));

    writer.release();
}
