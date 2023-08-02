#include <catch2/catch.hpp>
#include "../src/format.hpp" // Include the header containing the 'toLower' function

TEST_CASE("Test 'toLower' function", "[toLower]")
{
    REQUIRE(toLower("Hello, World!") == "hello, world!");
    REQUIRE(toLower("AbCdEfG") == "abcdefg");
    REQUIRE(toLower("") == "");
    REQUIRE(toLower("ALL UPPERCASE") == "all uppercase");
    REQUIRE(toLower("all lowercase") == "all lowercase");
}
