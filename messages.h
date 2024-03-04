#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <stdint.h>

typedef struct
{
    uint32_t id;
    void *   msg;
} cPS_Base_Packet_t;

#define cPS_ChargingStatus_MID (0xA80FD316)

typedef struct
{
    float input_v;
    float input_a;
    float battery_v;
    float battery_a;
} cPS_ChargingStatus_Packet_t;

#endif
