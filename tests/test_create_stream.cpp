#include <catch2/catch.hpp>
#include "../src/json.hpp"
#include "../src/stream_utils.hpp" // Include the header containing the 'create_stream' function

using json = nlohmann::json;

TEST_CASE("Test 'create_stream' function", "[create_stream]")
{
    // Create a temporary test directory containing some sample image and video files
    std::vector<std::string> contentPaths = {
        "../data/contents/examples/2.jpg",
        "../data/contents/examples/3.jpg",
        "../data/contents/examples/6.png"
        // Add more content paths here for testing different scenarios
    };

    json config = R"(
        {
            "fps": 30,
            "out": "autovideosink",
            "width": 640,
            "height": 480,
            "image_delay": 1,
            "play_infinite": false
        }
    )"_json;

    int result = create_stream(contentPaths, config);
    REQUIRE(result == 0);
}
