#include "catch2/catch.hpp"
#include "hiccup/infra/type_id.h"

using namespace hiccup;

namespace {
    struct Foo {
        int x;
    };

    struct Bar {
        int y;
    };
}

TEST_CASE( "Test Type Id" ) {
    SECTION( "should get type id of basic types ") {
        static_assert(TypeIdOf<int>() == TypeIdOf<int>(), "int");
        static_assert(TypeIdOf<char>() == TypeIdOf<char>(), "char");
        static_assert(TypeIdOf<float>() == TypeIdOf<float>(), "float");
        static_assert(TypeIdOf<double>() == TypeIdOf<double>(), "double");
        static_assert(TypeIdOf<void>() == TypeIdOf<void>(), "void");
        static_assert(TypeIdOf<int>() != TypeIdOf<char>(), "int != char");
    }

    SECTION( "should get the same id for the same type" ) {
        static_assert(TypeIdOf<Foo>() == TypeIdOf<Foo>(), "Foo");
        static_assert(TypeIdOf<Bar>() == TypeIdOf<Bar>(), "Bar");
    }

    SECTION( "should get different id for different type" ) {
        static_assert(TypeIdOf<Foo>() != TypeIdOf<Bar>(), "Foo != Bar");
    }
}