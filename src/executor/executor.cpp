#include "executor.h"
#include "alert.h"

void Executor::turn_left() {
	// static_cast 或其他显式转换方法将 int 转换为枚举类型
	this->pos.heading =  static_cast<Direction>((this->pos.heading + NR_DIR - 1) % NR_DIR);
}

void Executor::turn_right() {
	// static_cast 或其他显式转换方法将 int 转换为枚举类型
	this->pos.heading =  static_cast<Direction>((this->pos.heading + 1) % NR_DIR);
}

Position Executor::get_position() {
	return this->pos;
}

void Executor::forward() {
	switch (this->pos.heading) {
	case DIR_EAST:
		this->pos.point.x += 1;
		break;
	case DIR_NORTH:
		this->pos.point.y += 1;
		break;
	case DIR_WEST:
		this->pos.point.x -= 1;
		break;
	case DIR_SOUTCH:
		this->pos.point.y -= 1;
		break;
	}
}

void Executor::backward() {
	switch (this->pos.heading) {
	case DIR_EAST:
		this->pos.point.x -= 1;
		break;
	case DIR_NORTH:
		this->pos.point.y -= 1;
		break;
	case DIR_WEST:
		this->pos.point.x += 1;
		break;
	case DIR_SOUTCH:
		this->pos.point.y += 1;
		break;
	}
}

void Executor::do_step(Action act) {
	switch(act) {
	case GO_FORWARD:
		forward(); break;
	case GO_BACKWARD:
		backward(); break;
	case TURN_LEFT:
		turn_left(); break;
	case TURN_RIGHT:
		turn_right(); break;
	}

	if (in_danger()) {
		alert(IN_DANGEROUS, this->pos.point);
	}
	if (need_clean()) {
		alert(IN_CLEAN, this->pos.point);
		this->clean_points[this->pos.point] = false;
	}
}

void Executor::set_danger_point(Point p) {
	this->danger_points.insert(p);
}

bool Executor::is_danger_point(Point p) {
	return this->danger_points.count(p) > 0;
}

bool Executor::in_danger() {
	return is_danger_point(get_position().point);
}

void Executor::set_clean_point(Point p) {
	// "true" means need clean initially
	this->clean_points.insert(std::make_pair(p, true));
}

bool Executor::is_clean_point(Point p) {
	return (this->clean_points.count(p) > 0);
}

bool Executor::need_clean() {
	return is_clean_point(this->pos.point) && (this->clean_points[this->pos.point]);
}