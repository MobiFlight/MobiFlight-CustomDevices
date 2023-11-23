#pragma once

#include <Arduino.h>
#include "../KAV_Simulation/KAV_A3XX_FCU_LCD.h"
#include "../KAV_Simulation/KAV_A3XX_EFIS_LCD.h"
#include "../Mobiflight/GNC255/GNC255.h"
#include "../Mobiflight/GenericI2C/GenericI2C.h"

enum {
    KAV_FCU,
    KAV_EFIS,
    MOBIFLIGHT_GNC255,
    MOBIFLIGHT_GENERICI2C
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
    bool               getStringFromEEPROM(uint16_t addreeprom, char *buffer);
    bool               _initialized = false;
    KAV_A3XX_FCU_LCD  *_FCU_LCD;
    KAV_A3XX_EFIS_LCD *_EFIS_LCD;
    GNC255            *_GNC255_OLED;
    GenericI2C        *_myGenericI2C;
    uint8_t            _customType;
};
