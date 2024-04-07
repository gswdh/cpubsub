#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <stdint.h>

typedef struct __attribute__((packed)) {
  uint mid;
  float vbat_v;
  float vcells_v[4];
} MSGBatteryVoltage_t;
#define MSGBatteryVoltage_MID (0x1)
#define MSGBatteryVoltage_MID_LEN (sizeof(MSGBatteryVoltage_MID))

typedef struct __attribute__((packed)) {
  uint mid;
  ubyte power_state;
} MSGPowerStatus_t;
#define MSGPowerStatus_MID (0x2)
#define MSGPowerStatus_MID_LEN (sizeof(MSGPowerStatus_MID))

#endif
