#include "GenericI2C.h"
#include <Wire.h>

#define END_OF_I2C_MESSAGE              0x00
#define END_OF_I2C_COMMAND              0x0D        // carriage return in ASCII
#define END_OF_I2C_PARTIAL_MESSAGE      0x0A        // line feed in ASCII
#if defined(ARDUINO_ARCH_RP2040)
  #define BUFFER_LENGTH                 WIRE_BUFFER_SIZE
#endif

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


void GenericI2C::set(int16_t messageID, char *setPoint)
{
    /* **********************************************************************************
        MessageID and setpoint will be send via I2C
        For AVR's the I2C buffer is only 32 bytes, so message gets spilt up if exceeding
        max. length of a message could be 96 bytes due to limitation from the CMDmessenger
        Important Remark!
        MessageID == -1 will be send from the connector when Mobiflight is closed
        MessageID == -2 will be send from the connector when PowerSavingMode is entered
    ********************************************************************************** */
    char buffer[7] = {0};
    uint8_t countChar = 0;
	uint8_t countI2C = 0;

    itoa(messageID, buffer, 10);
    Wire.beginTransmission(_addrI2C);
    Wire.print(buffer);
    Wire.write(END_OF_I2C_COMMAND);                                     // send a CR to mark end of command
    countI2C = strlen(buffer) + 1;                                      // count already transferred bytes incl. end of command marker
    while (countChar < strlen(setPoint)) {
        Wire.write(setPoint[countChar++]);
        countI2C++;
        if (countI2C >= (BUFFER_LENGTH - 1)) {							// if buffer will be exceeded on next characater, keep one byte for end of message marker
            Wire.write(END_OF_I2C_PARTIAL_MESSAGE);                     // send a LF for next part of message
			Wire.endTransmission();								        // write buffer to I2C display
			Wire.beginTransmission(_addrI2C);							// and prepare a new transmission
			countI2C = 0;												// start new Byte counting
		}
    }
    Wire.write(END_OF_I2C_MESSAGE);                                     // send a NULL to mark end of messageID
    Wire.endTransmission();
}

void GenericI2C::update()
{
    // Do something which is required regulary
}
