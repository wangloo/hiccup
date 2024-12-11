#include "catch2/catch.hpp"
#include "executor/point.h"

using namespace hiccup;

TEST_CASE( "Test Point" ) {
    SECTION( "should compare point" ) {
        Point p1(1, 2);
        Point p2(1, 2);
        Point p3(2, 3);
        REQUIRE(p1 == p2);
        REQUIRE(p1 != p3);
    }

    SECTION( "should convert point to string" ) {
        Point p(1, 2);
        REQUIRE(p.ToString() == "(1, 2)");
    }
}