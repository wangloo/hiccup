/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef WIFI_H
#define WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct WifiMsg{
	int command, x, y, action;
} WifiMsg;

void send_msg(const WifiMsg* msg);
void send_json(const char* json);

#ifdef __cplusplus
}
#endif

#endif
