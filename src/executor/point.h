/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef POINT_H
#define POINT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Point {
	int x;
	int y;
} Point;

Point PointNew(int x, int y);
bool PointEquals(const Point*, const Point*);
const char* PointToString(const Point*);

#ifdef __cplusplus
}
#endif

#endif
