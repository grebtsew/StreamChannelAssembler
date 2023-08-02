#include <catch2/catch.hpp>
#include "../src/json.hpp"
#include "../src/stream_utils.hpp" // Include the header containing the 'create_stream' function

using json = nlohmann::json;

TEST_CASE("Test 'create_stream' function", "[create_stream]")
{
    // Create a temporary test directory containing some sample image and video files
    std::vector<std::string> contentPaths = {
        "./data/contents/2.jpg"
        // Add more content paths here for testing different scenarios
    };

    json config = R"(
        {
            "fps": 30,
            "out": "test_output_video.avi",
            "width": 640,
            "height": 480,
            "image_delay": 500,
            "play_infinite": false
        }
    )"_json;

    int result = create_stream(contentPaths, config);
    REQUIRE(result == 0);

    // Add more test cases here to cover different scenarios and configurations

    // Optionally, you can also check the output file to verify the correctness of the created video

    // Clean up the test output file
    std::remove("test_output_video.avi");
}
