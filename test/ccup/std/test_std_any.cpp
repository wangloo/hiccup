#include "catch2/catch.hpp"
#include <any>
#include <unordered_map>

using namespace std;

SCENARIO("Test std::any") {
    GIVEN("An any object") {
        any a = 1;
        REQUIRE(any_cast<int>(a) == 1);
        REQUIRE_THROWS_AS(any_cast<float>(a), bad_any_cast);
    }

    GIVEN("An any object with a string") {
        any a = string("hello");
        REQUIRE(any_cast<string>(a) == "hello");
        REQUIRE(any_cast<string>(&a) != nullptr);
    }

    GIVEN("Could save any in hashmap") {
        unordered_map<string, any> m;
        m["int"] = 1;
        m["float"] = 1.0f;
        m["string"] = string("hello");

        REQUIRE(any_cast<int>(m["int"]) == 1);
        REQUIRE(any_cast<float>(m["float"]) == 1.0f);
        REQUIRE(any_cast<string>(m["string"]) == "hello");
    }
}
