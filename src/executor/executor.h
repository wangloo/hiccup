#ifndef EXECUTOR_H
#define EXECUTOR_H

#ifdef __cplusplus
extern "C" {
#endif

enum Direction {
	DIR_EAST = 0, DIR_SOUTCH, DIR_WEST, DIR_NORTH, NR_DIR
};

struct Position {
	int x;
	int y;
	Direction heading;

	Position(int _x, int _y, Direction _heading)
	: x(_x), y(_y), heading(_heading) {

	}
	// 重载 == 运算符为成员函数
    bool operator==(const Position& other) const {
        return (x == other.x) && (y == other.y)  && (heading == other.heading);
    }
};
class Executor {
private:
	Position pos;
public:
	Executor(Position _pos = Position(0, 0, DIR_NORTH)): pos(_pos) {}
	void turn_left(void);
	void turn_right(void);
	Position get_position(void);
	void forward(void);
	void backward(void);
};


#ifdef __cplusplus
}
#endif

#endif
