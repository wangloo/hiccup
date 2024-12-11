#include "point.h"
#include <stdio.h>
#include <stdlib.h>

Point PointNew(int x, int y) {
	Point p;
	p.x = x;
	p.y = y;
	return p;
}

bool PointEquals(const Point *p1, const Point *p2) {
	return p1->x == p2->x && p1->y == p2->y;
}

const char* PointToString(const Point* p) {
    // Allocate memory for the resulting string
    // Assuming integers are at most 11 characters (including sign)
    char* result = (char*)malloc(32); // Enough space for "(x, y)\0"
    if (result == NULL) {
        return NULL;
    }

    // Format the string
    snprintf(result, 32, "(%d, %d)", p->x, p->y);
    return result;
}