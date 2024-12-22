#include "catch2/catch.hpp"
#include "executor/executor.h"
#include "executor/alert.h"
#include <string.h>

TEST_CASE( "Test Executor" ) {
    SECTION( "should init" ) {
		// 隐式init
		Executor exec;
		REQUIRE(exec.get_position() == Position(Point(0, 0), DIR_NORTH));
    }

    SECTION( "should init2" ) {
		// 显式 init
		Executor exec = Executor(Position(Point(1, 2), DIR_EAST));
		REQUIRE(exec.get_position() == Position(Point(1, 2), DIR_EAST));
	}

	SECTION( "test turn left/right" ) {
		Executor exec = Executor(Position(Point(1, 2), DIR_EAST));
		exec.do_step(TURN_LEFT);
		REQUIRE(exec.get_position().heading == DIR_NORTH);
		exec.do_step(TURN_RIGHT);
		REQUIRE(exec.get_position().heading == DIR_EAST);
	}
	
	SECTION( "test forward/backward" ) {
		Executor exec = Executor(Position(Point(1, 2), DIR_EAST));
		exec.do_step(GO_FORWARD);
		REQUIRE(exec.get_position() == Position(Point(2, 2), DIR_EAST));
		exec.do_step(TURN_LEFT);
		exec.do_step(GO_BACKWARD);
		REQUIRE(exec.get_position() == Position(Point(2, 1), DIR_NORTH));
	}

	SECTION( "test set dangerous points" ) {
		Executor exec = Executor(Position(Point(1, 2), DIR_EAST));
		Point p = Point(2, 2);

		exec.set_danger_point(p);
		REQUIRE(exec.is_danger_point(p));
		exec.do_step(GO_FORWARD);
		REQUIRE(exec.in_danger());
		REQUIRE(assert_alert_event(IN_DANGEROUS, exec.get_position().point, 1));
		exec.do_step(TURN_LEFT);
		REQUIRE(assert_alert_event(IN_DANGEROUS, exec.get_position().point, 2));
		exec.do_step(GO_FORWARD);
		REQUIRE(assert_alert_event(IN_DANGEROUS, p, 2));
		REQUIRE(assert_alert_event(IN_DANGEROUS, exec.get_position().point, 0));
		exec.do_step(GO_BACKWARD);
		REQUIRE(assert_alert_event(IN_DANGEROUS, exec.get_position().point, 3));

		alert_reset();
	}
	
	SECTION( "test clean points" ) {
		Executor exec = Executor(Position(Point(1, 2), DIR_EAST));
		Point clean_p = Point(2, 2);
		exec.set_clean_point(clean_p);
		REQUIRE(exec.is_clean_point(clean_p));

		// assert 事件发生
		exec.do_step(GO_FORWARD);
		REQUIRE(assert_alert_event(IN_CLEAN, exec.get_position().point, 1));
		// assert 事件数量没变
		exec.do_step(TURN_LEFT);
		REQUIRE(assert_alert_event(IN_CLEAN, exec.get_position().point, 1));
		// assert 事件数量没变
		exec.do_step(GO_FORWARD);
		exec.do_step(GO_BACKWARD);
		REQUIRE(assert_alert_event(IN_CLEAN, exec.get_position().point, 1));

		alert_reset();
	}
}