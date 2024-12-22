/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ALERT_H
#define ALERT_H
#include "point.h"


typedef enum {
	IN_DANGEROUS = 3,
	IN_CLEAN,
	
} AlertType;

void alert(AlertType type, Point p);
bool assert_alert_event(AlertType type, Point p, int expected_time);
void alert_reset(void);


#endif
