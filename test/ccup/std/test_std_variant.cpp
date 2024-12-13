#include "catch2/catch.hpp"
#include <variant>
#include <vector>
#include <iostream>

using namespace std;

SCENARIO("Test std::variant") {
    GIVEN("A variant object") {

        variant<int, float> v = 1;
        REQUIRE(get<int>(v) == 1);
        REQUIRE_THROWS_AS(get<float>(v), bad_variant_access);

        v = 1.0f;
        REQUIRE(get<float>(v) == 1.0f);
        REQUIRE_THROWS_AS(get<int>(v), bad_variant_access);
    }

    GIVEN("A variant object with a float") {
        variant<int, float> v = 1.0f;
        REQUIRE(get<float>(v) == 1.0f);
        REQUIRE_THROWS_AS(get<int>(v), bad_variant_access);
    }

    GIVEN("A variant object could be index") {
        variant<int, float> v = 1;
        REQUIRE(v.index() == 0);

        v = 1.0f;
        REQUIRE(v.index() == 1);
    }

    GIVEN("A variant object could be visit") {
        variant<int, float> v = 1;
        auto result = visit([](auto&& arg) -> string {
            using T = decay_t<decltype(arg)>;
            if constexpr (is_same_v<T, int>) {
                return "int";
            } else if constexpr (is_same_v<T, float>) {
                return "float";
            } else {
                return "unknown";
            }
        }, v);
        REQUIRE(result == "int");

        v = 1.0f;
        result = visit([](auto&& arg) -> string {
            using T = decay_t<decltype(arg)>;
            if constexpr (is_same_v<T, int>) {
                return "int";
            } else if constexpr (is_same_v<T, float>) {
                return "float";
            } else {
                return "unknown";
            }
        }, v);
        REQUIRE(result == "float");
    }
}


struct Triangle {
    void draw() const {
        std::cout << "△" << std::endl;
    }
};

struct Circle {
    void draw() const {
        std::cout << "○" << std::endl;
    }
};

TEST_CASE("Test std::variant with polymorphic behavior") {
    using Shape = std::variant<Triangle, Circle>;
    std::vector<Shape> shapes {Circle{}, Triangle{}};
    for (const auto &shape : shapes) {
        std::visit([](const auto &s) { s.draw(); }, shape);
    }
}

namespace {
    struct Dog {
        string bark() const {
            return "woof";
        }
    };

    struct Cat {
        string meow() const {
            return "meow";
        }
    };

    struct Aminal {
        Aminal(Dog d) : a(d) {}
        Aminal(Cat c) : a(c) {}

        string sound() const {
            return visit([](auto&& arg) -> string {
                using T = decay_t<decltype(arg)>;
                if constexpr (is_same_v<T, Dog>) {
                    return arg.bark();
                } else if constexpr (is_same_v<T, Cat>) {
                    return arg.meow();
                } else {
                    return "unknown";
                }
            }, a);
        }
    private:
        variant<Dog, Cat> a;
    };
}

SCENARIO("Test static dispatching by std::variant") {
    GIVEN("An animal object") {
        Dog d;
        Aminal a(d);
        REQUIRE(a.sound() == "woof");

        Cat c;
        a = c;
        REQUIRE(a.sound() == "meow");
    }

    GIVEN("An animal container") {
        vector<Aminal> animals;
        animals.emplace_back(Dog());
        animals.emplace_back(Cat());

        REQUIRE(animals[0].sound() == "woof");
        REQUIRE(animals[1].sound() == "meow");
    }
}
