/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef ALERT_H
#define ALERT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	IN_DANGEROUS = 3,
} AlertType;

void alert(AlertType type, int x, int y);

#ifdef __cplusplus
}
#endif

#endif
