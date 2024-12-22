/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef POINT_H
#define POINT_H

struct Point {
	int x, y;

	Point(int _x, int _y)
	: x(_x), y(_y) {
	}

	bool operator==(const Point& other) const {
		return (x == other.x) && (y == other.y);
	}
    // 重载 < 运算符，以便 set 能够排序，set依赖这个
    bool operator<(const Point& other) const {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }
};

#endif
