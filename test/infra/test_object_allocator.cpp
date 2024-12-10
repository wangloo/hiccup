#include "catch2/catch.hpp"
#include "hiccup/infra/object_allocator.h"

using namespace hiccup;

namespace {
    struct Foo: LinkNode<Foo> {
        Foo(int a) :
                x(a) {
        }

        int value() const {
            return x;
        }

        void value(int v) {
            this->x = v;
        }

        ~Foo() {
            this->x = 0;
        }

    private:
        int x;
    };
}

TEST_CASE( "Test Object Allocator" ) {
	const size_t FOO_MAX = 3;
    ObjectAllocator<Foo> fooAllocator{FOO_MAX};

    SECTION( "should alloc and free without construct and destruct" ) {
        REQUIRE(fooAllocator.GetAvailableSize() == FOO_MAX);

        auto f = new (fooAllocator.Alloc()) Foo(5);
        REQUIRE(f != nullptr);
        REQUIRE(f->value() == 5);
        REQUIRE(fooAllocator.GetAvailableSize() == FOO_MAX - 1);

        fooAllocator.Free(*f);
        REQUIRE(f->value() == 5);
        REQUIRE(fooAllocator.GetAvailableSize() == FOO_MAX);
    }

    SECTION( "should new construct delete destruct" ) {
        REQUIRE(fooAllocator.GetAvailableSize() == FOO_MAX);

        auto f = fooAllocator.New(5);
        REQUIRE(f != nullptr);
        REQUIRE(f->value() == 5);
        REQUIRE(fooAllocator.GetAvailableSize() == FOO_MAX - 1);

        fooAllocator.Delete(*f);
        REQUIRE(f->value() == 0);
        REQUIRE(fooAllocator.GetAvailableSize() == FOO_MAX);
    }

    SECTION( "should extend auto" ) {
        Foo *foos[FOO_MAX + 1] = { nullptr };

        for (size_t i = 0; i < FOO_MAX; i++) {
            foos[i] = fooAllocator.New(i + 1);
            REQUIRE(foos[i] != nullptr);
        }

        REQUIRE(fooAllocator.GetAvailableSize() == 0);

        foos[FOO_MAX] = fooAllocator.New(FOO_MAX + 1);
        REQUIRE(foos[FOO_MAX] != nullptr);
        REQUIRE(foos[FOO_MAX]->value() == FOO_MAX + 1);
        REQUIRE(fooAllocator.GetAvailableSize() == 0);

        fooAllocator.Delete(*foos[0]);
        REQUIRE(fooAllocator.GetAvailableSize() == 1);

        foos[0] = fooAllocator.New(1);
        REQUIRE(foos[0] != nullptr);
        REQUIRE(fooAllocator.GetAvailableSize() == 0);

        for (auto &f : foos) {
            fooAllocator.Delete(*f);
        }
        REQUIRE(fooAllocator.GetAvailableSize() == 4);
    }

}
