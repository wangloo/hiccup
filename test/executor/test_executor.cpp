#include "catch2/catch.hpp"
#include "executor/executor.h"
#include <string.h>

TEST_CASE( "Test Executor" ) {
    SECTION( "should init" ) {
		// 隐式init
		Executor exec;
		REQUIRE(exec.get_position() == Position(0, 0, DIR_NORTH));
    }

    SECTION( "should init2" ) {
		// 显式 init
		Executor exec = Executor(Position(1, 2, DIR_EAST));
		REQUIRE(exec.get_position() == Position(1, 2, DIR_EAST));
	}

	SECTION( "test turn left/right" ) {
		Executor exec = Executor(Position(1, 2, DIR_EAST));
		exec.turn_left();
		REQUIRE(exec.get_position().heading == DIR_NORTH);
		exec.turn_right();
		REQUIRE(exec.get_position().heading == DIR_EAST);
	}
	
	SECTION( "test forward/backward" ) {
		Executor exec = Executor(Position(1, 2, DIR_EAST));
		exec.forward();
		REQUIRE(exec.get_position() == Position(2, 2, DIR_EAST));
		exec.turn_left();
		exec.backward();
		REQUIRE(exec.get_position() == Position(2, 1, DIR_NORTH));
	}

}