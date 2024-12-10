#include "catch2/catch.hpp"
#include "hiccup/infra/enum_cast.h"

using namespace hiccup;

namespace {
    enum class TestEnum {
        A = 1,
        B = 2,
        C = 3,
    };
}

TEST_CASE( "Test Enum Cast" ) {
    SECTION( "should cast enum to int" ) {
        REQUIRE(enum_id_cast(TestEnum::A) == 1);
        REQUIRE(enum_id_cast(TestEnum::B) == 2);
        REQUIRE(enum_id_cast(TestEnum::C) == 3);
    }
}