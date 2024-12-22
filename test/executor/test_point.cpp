#include "catch2/catch.hpp"
#include "executor/point.h"
#include <string.h>

TEST_CASE( "Test Point" ) {
    SECTION( "should compare point" ) {
        Point p1 = Point(1, 2);
        Point p2 = Point(1, 2);
        Point p3 = Point(2, 3);

        REQUIRE(p1 == p2);
        REQUIRE(!(p1 == p3));
    }

//     SECTION( "should convert point to string" ) {
//         Point p = PointNew(1, 2);
//         const char* str = PointToString(&p);
//         REQUIRE(str != nullptr);
//         REQUIRE(strcmp(str, "(1, 2)") == 0);
//         free((void*)str);
//    }
}