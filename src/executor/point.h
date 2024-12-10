/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef POINT_H
#define POINT_H

#include "hiccup/infra/eq_helper.h"
#include <string>

namespace hiccup {

struct Point {
	Point();
	Point(int x, int y);

	__INLINE_EQUALS(Point) {
        return __FIELD_EQ(x) && __FIELD_EQ(y);
    }

	std::string ToString() const;

private:
	int x{0};
	int y{0};
};

}

#endif
