#include "catch2/catch.hpp"
#include <any>
#include <optional>
#include <variant>

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
}

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

SCENARIO("Test std::variant") {
    GIVEN("A variant object") {
        variant<int, float> v = 1;
        REQUIRE(get<int>(v) == 1);
        REQUIRE_THROWS_AS(get<float>(v), bad_variant_access);
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

    GIVEN("An animal container dispatching by std::variant") {
        vector<Aminal> animals;
        animals.emplace_back(Dog());
        animals.emplace_back(Cat());

        REQUIRE(animals[0].sound() == "woof");
        REQUIRE(animals[1].sound() == "meow");
    }
}