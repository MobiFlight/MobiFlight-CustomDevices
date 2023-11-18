#include "GenericI2C.h"
#include "allocateMem.h"
#include "commandmessenger.h"
#include <Wire.h>

/* **********************************************************************************
    This is just the basic code to set up your custom device.
    Change/add your code as needed.
********************************************************************************** */

GenericI2C::GenericI2C(uint8_t addrI2C)
{
    _addrI2C = addrI2C;
    _initialised = true;
}

void GenericI2C::begin()
{
    Wire.begin();
    Wire.setClock(400000);
}

void GenericI2C::attach()
{
}

void GenericI2C::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

void GenericI2C::set(int8_t messageID, char *setPoint)
{
    /* **********************************************************************************
        MessageID and setpoint will be send via I2C
        Important Remark!
        MessageID == -1 will be send from the connector when Mobiflight is closed
        MessageID == -2 will be send from the connector when PowerSavingMode is entered
    ********************************************************************************** */
    char buffer[7] = {0};
    itoa(messageID, buffer, 10);
    Wire.beginTransmission(_addrI2C);
    Wire.print(buffer);
    Wire.write(0x0D);   // send a CR to mark end of command
    Wire.print(setPoint);
    Wire.write(0x00);   // send a NULL to mark end of messageID
    Wire.endTransmission();
}

void GenericI2C::update()
{
    // Do something which is required regulary
}
