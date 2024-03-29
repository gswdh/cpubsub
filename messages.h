#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <stdint.h>

#define CPS_CHARGING_STATUS_MID (0x00000000)

typedef struct
{
    uint32_t mid;
    float    input_v;
    float    input_a;
    float    battery_v;
    float    battery_a;
} cps_charging_status_packet_t;

#define CPS_POWER_REQUEST_MID (0x00000001)

typedef struct
{
    uint32_t mid;
    bool     pwr_dwn_req;
} cps_power_request_packet_t;

#define CPS_UART_TERMINAL_MID (0x00000002)

typedef struct
{
	uint32_t mid;
	char message[256];
} cps_uart_terminal_packet_t;

static const uint32_t cps_msg_size[] = {sizeof(cps_charging_status_packet_t), sizeof(cps_power_request_packet_t), sizeof(cps_uart_terminal_packet_t)};

#endif
