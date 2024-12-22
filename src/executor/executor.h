#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "point.h"
#include <set>
#include <map>

#ifdef __cplusplus
extern "C" {
#endif

enum Direction {
	DIR_EAST = 0, DIR_SOUTCH, DIR_WEST, DIR_NORTH, NR_DIR
};

enum Action {
	GO_FORWARD = 0,
	GO_BACKWARD,
	TURN_LEFT,
	TURN_RIGHT,
};

struct Position {
	Point point;
	Direction heading;

	Position(Point _point, Direction _heading)
	: point(_point), heading(_heading) {

	}
	// 重载 == 运算符为成员函数
    bool operator==(const Position& other) const {
        return (point == other.point)  && (heading == other.heading);
    }
};

class Executor {
private:
	Position pos;
	std::set<Point> danger_points;
	std::map<Point, bool> clean_points; // (point, need clean?)
	void turn_left(void);
	void turn_right(void);
	void forward(void);
	void backward(void);
public:
	Executor(Position _pos = Position(Point(0, 0), DIR_NORTH)): 
	pos(_pos) {
		this->danger_points.clear();
		this->clean_points.clear();
	}
	Position get_position(void);
	void do_step(Action act);
	void set_danger_point(Point p);
	bool is_danger_point(Point p);
	bool in_danger();
	void set_clean_point(Point p);
	bool is_clean_point(Point p);
	bool need_clean();
	// void set_cleaned();
};


#ifdef __cplusplus
}
#endif

#endif
