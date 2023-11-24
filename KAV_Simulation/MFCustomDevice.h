#pragma once

#include <Arduino.h>
#include "KAV_A3XX_FCU_LCD.h"
#include "KAV_A3XX_EFIS_LCD.h"
#include "KAV_A3XX_BATTERY_LCD.h"
#include "KAV_A3XX_RAD_TCAS_LCD.h"
#include "KAV_A3XX_RUDDER_LCD.h"

enum {
    KAV_FCU = 1,
    KAV_EFIS,
    KAV_BATTERY,
    KAV_RAD_TCAS,
    KAV_RUDDER
};
class MFCustomDevice
{
public:
    MFCustomDevice();
    void attach(uint16_t adrPin, uint16_t adrType, uint16_t adrConfig);
    void detach();
    void update();
    void set(int16_t messageID, char *setPoint);

private:
    bool                    getStringFromEEPROM(uint16_t addreeprom, char *buffer);
    bool                    _initialized = false;
    KAV_A3XX_FCU_LCD        *_FCU_LCD;
    KAV_A3XX_EFIS_LCD       *_EFIS_LCD;
    KAV_A3XX_BATTERY_LCD    *_BATTERY_LCD;
    KAV_A3XX_RAD_TCAS_LCD   *_RAD_TCAS_LCD;
    KAV_A3XX_RUDDER_LCD     *_RUDDER_LCD;
    uint8_t                 _lcdType;
};
