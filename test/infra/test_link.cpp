#include "catch2/catch.hpp"
#include "hiccup/infra/link.h"
#include <iostream>

using namespace hiccup;

namespace {
    struct Foo : LinkNode<Foo> {
        Foo(int a) : x(a) {
        }

        int value() const {
            return x;
        }

        void value(int v) {
        	this->x = v;
        }

    private:
        int x;
    };
}

TEST_CASE( "Test Link" ) {
	Link<Foo> link;

    SECTION( "should be empty when init" ) {
        REQUIRE(link.front() == nullptr);
        REQUIRE(link.back() == nullptr);
        REQUIRE(link.empty());
        REQUIRE(link.size() == 0);
        REQUIRE(link.begin() == link.end());
    }

    SECTION( "should push back and front" ) {
        Foo elem1(1);
        Foo elem2(2);
        Foo elem3(3);

        link.push_back(elem1);
        link.push_back(elem2);
        link.push_front(elem3);

        REQUIRE(link.size() == 3);

        Foo* first = link.pop_front();
        REQUIRE(first->value() == 3);

        REQUIRE(link.front()->value() == 1);
        REQUIRE(link.back()->value() == 2);
    }

    SECTION( "get elem when link is not empty" ) {
        Foo elem(1);

        link.push_back(elem);

        REQUIRE(!link.empty());
        REQUIRE(link.size() == 1);
        REQUIRE(link.front() == &elem);
        REQUIRE(link.back() == &elem);

        Foo* first = link.pop_front();
        REQUIRE(first->value() == 1);
        REQUIRE(link.empty());
    }

    SECTION( "travel link by order" ) {
        Foo elem1(1), elem2(2), elem3(3);

        link.push_back(elem1);
        link.push_back(elem2);
        link.push_back(elem3);

        int i = 1;
        for (const auto& node : link) {
            REQUIRE(node.value() == i++);
        }
    }

    SECTION( "travel link by reverse order" ) {
        Foo elem1(1), elem2(2), elem3(3);

        link.push_back(elem1);
        link.push_back(elem2);
        link.push_back(elem3);

        int i = link.size();

        for (auto it = link.rbegin(); it != link.rend(); ++it) {
            REQUIRE(it->value() == i--);
        }
    }

    SECTION( "travel link by modifing" ) {
        Foo elem1(1), elem2(2), elem3(3);

        link.push_back(elem1);
        link.push_back(elem2);
        link.push_back(elem3);

        for (auto& node : link) {
            node.value(node.value() * 2);
        }

        int i = link.size() * 2;
        int sum = 0;

        for (auto it = link.rbegin(); it != link.rend(); ++it) {
            REQUIRE(it->value() == i);
            sum += it->value();
            i -= 2;
        }

        REQUIRE(sum == 12);
    }

    SECTION( "should point to the correct addr when get next" ) {
        Foo elem(1);
        link.push_back(elem);

        REQUIRE(link.begin().value() == &elem);
        REQUIRE(link.end().value() != &elem);
        Link<Foo>::Iterator p = link.begin();
        REQUIRE(link.next_of(p) == link.end());
    }

    SECTION( "should only move link" ) {
        Link<Foo> link1;
        
        Foo elem1(1), elem2(2), elem3(3);

        link1.push_back(elem1);
        link1.push_back(elem2);
        link1.push_back(elem3);

        Link<Foo> link2{std::move(link1)};

        REQUIRE(link1.empty());
        REQUIRE(link2.size() == 3);
        REQUIRE(link2.front()->value() == 1);
        REQUIRE(link2.back()->value() == 3);
    }
}
