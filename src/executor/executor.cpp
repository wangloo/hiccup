#include "executor.h"

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
		this->pos.x += 1;
		return;
	case DIR_NORTH:
		this->pos.y += 1;
		return;
	case DIR_WEST:
		this->pos.x -= 1;
		return;
	case DIR_SOUTCH:
		this->pos.y -= 1;
		return;
	}
}

void Executor::backward() {
	switch (this->pos.heading) {
	case DIR_EAST:
		this->pos.x -= 1;
		return;
	case DIR_NORTH:
		this->pos.y -= 1;
		return;
	case DIR_WEST:
		this->pos.x += 1;
		return;
	case DIR_SOUTCH:
		this->pos.y += 1;
		return;
	}
}
	