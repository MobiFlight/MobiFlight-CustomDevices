#pragma once

#include <Arduino.h>
#include "KAV_A3XX_FCU_LCD.h"
#include "KAV_A3XX_EFIS_LCD.h"

enum {
    KAV_FCU = 1,
    KAV_EFIS
};
class MFCustomDevice
{
public:
    MFCustomDevice(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig);
    void detach();
    void update();
    void set(int8_t messageID, char *setPoint);

private:
    bool               getStringFromEEPROM(uint16_t addreeprom, char *buffer);
    bool               _initialized = false;
    KAV_A3XX_FCU_LCD  *_FCU_LCD;
    KAV_A3XX_EFIS_LCD *_EFIS_LCD;
    uint8_t            _lcdType;
};
