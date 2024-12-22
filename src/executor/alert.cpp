#include "alert.h"
#include "point.h"
#include <stdio.h>
#include <map>
#include <vector>

static std::map<AlertType, std::vector<Point>> alert_events;

void alert(AlertType type, Point p) {
	printf("ALLERT!! type: %d, Point: (%d, %d)\n", type, p.x, p.y);
	alert_events[type].emplace_back(p);
}

bool assert_alert_event(AlertType type, Point p, int expected_time) {
	int time, i;

	if (!(alert_events.count(type) > 0)) {
		return false;
	} 
	for (i = 0, time = 0; i < alert_events[type].size(); i++) {
		if (alert_events[type][i] == p)
			time += 1;
	}
	return time == expected_time;
}

void alert_reset(void) {
	alert_events.clear();
}
	