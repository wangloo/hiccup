#include "catch2/catch.hpp"
#include <optional>

using namespace std;

SCENARIO("Test std::optional") {
    GIVEN("An empty optional object") {
        optional<int> o;
        REQUIRE_FALSE(o.has_value());
    }

    GIVEN("An optional object") {
        optional<int> o = 1;
        REQUIRE(o.has_value());
        REQUIRE(*o == 1);
    }

    GIVEN("An optional object could be reset") {
        optional<string> o = "hello";
        REQUIRE(o.has_value());
        o.reset();
        REQUIRE_FALSE(o.has_value());
    }

    GIVEN("An optional object could be value_or") {
        optional<string> o = "hello";
        REQUIRE(o.has_value());
        
        auto str = o.value_or("world");
        REQUIRE(str == "hello");

        o.reset();
        str = o.value_or("world");
        REQUIRE(str == "world");
    }
}
