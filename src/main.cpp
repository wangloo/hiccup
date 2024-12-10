#include <iostream>
#include "point.h"

int main() {
    hiccup::Point p(1, 2);
    std::cout << p.ToString() << std::endl;
    return 0;
}
