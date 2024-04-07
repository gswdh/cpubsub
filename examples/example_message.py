import ctypes

from message_base import CStructBase

class MSGBatteryVoltage(CStructBase):
    fields = [
        ("mid", ctypes.c_uint32),
        ("vbat_v", ctypes.c_float),
        ("vcells_v", ctypes.c_float * 4)
    ]
    def __init__(self, vbat_v, vcells_v):
        self.vbat_v = vbat_v
        self.vcells_v = vcells_v

class MSGPowerStatus(CStructBase):
    fields = [
        ("mid", ctypes.c_uint32),
        ("power_state", ctypes.c_uint8),
    ]
    def __init__(self, power_state):
        self.power_state = power_state

MSGS = {
    0x01:MSGBatteryVoltage,
    0x02:MSGPowerStatus,
}