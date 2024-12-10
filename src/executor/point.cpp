#include "point.h"

namespace hiccup {

Point::Point() {
}

Point::Point(int x, int y)
: x(x), y(y) {
}

std::string Point::ToString() const {
	return std::string("(") + std::to_string(x) + ", " + std::to_string(y) + std::string(")");
}

}