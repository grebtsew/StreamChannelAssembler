#include <catch2/catch.hpp>
#include "../src/format.hpp" // Include the header containing the 'getInputFormat' function

TEST_CASE("Test 'getInputFormat' function", "[getInputFormat]")
{
    REQUIRE(getInputFormat("image.jpg") == Image);
    REQUIRE(getInputFormat("video.mp4") == Video);
    REQUIRE(getInputFormat("unknown.txt") == Unknown);
}
