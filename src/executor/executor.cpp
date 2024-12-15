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
	