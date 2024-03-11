#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <stdint.h>

#define cPS_ChargingStatus_MID (0x00000000)

typedef struct
{
    uint32_t mid;
    float    input_v;
    float    input_a;
    float    battery_v;
    float    battery_a;
} cPS_ChargingStatus_Packet_t;

#define cPS_PowerRequest_MID (0x00000001)

typedef struct
{
    uint32_t mid;
    bool     pwr_dwn_req;
} cPS_PowerRequest_Packet_t;

static const uint32_t cPS_msg_size[] = {sizeof(cPS_ChargingStatus_Packet_t),
                                        sizeof(cPS_PowerRequest_Packet_t)};

#endif
