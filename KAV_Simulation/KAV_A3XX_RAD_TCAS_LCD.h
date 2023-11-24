/**
 * Kav A3XX RUDDER LCD
 * Written by: James Kavanagh & Keith Greenwood
 * This library has been written to drive the 'Kav Simulations' EFIS LCD Display.
 */

#pragma once

#define BUFFER_SIZE_MAX 16

#include "Arduino.h"
#include "HT1621.h"

class KAV_A3XX_RAD_TCAS_LCD {
private:
  // Fields
  HT1621 ht_rad_tcas;
  uint8_t buffer[BUFFER_SIZE_MAX];
  bool _initialised;
  byte _CS;
  byte _CLK;
  byte _DATA;

  // Methods
  void handleMobiFlightCmd(char *string);
  void displayDigit(uint8_t address, uint8_t digit);
  void refreshLCD(uint8_t address);
  
public:
  // Constructor
  // 'CLK' is sometimes referred to as 'RW'
  KAV_A3XX_RAD_TCAS_LCD(uint8_t CS, uint8_t CLK, uint8_t DATA) : ht_rad_tcas(CS, CLK, DATA) { };

  void begin();
  void clearLCD();
  void clearDigit(uint8_t address);
  void attach(byte CS, byte CLK, byte DATA);
  void detach();
  void set(int16_t messageID, char *setPoint);

  // Set Dot Function
  void setRadioDot(bool enabled);
  void setSpecificDot(uint8_t address, bool enabled);
  void setAllDots(bool enabled);
  // Set value function
  void setRadioValue(uint32_t value);
  void setTcasValue(uint16_t value);

  // Show Rudder Value function
  void showRadio(uint32_t value);
  void showTcas(uint16_t value);
  void showTest(bool enabled);
};
