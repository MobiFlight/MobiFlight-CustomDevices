#include "KAV_A3XX_RUDDER_LCD.h"

#define DIGIT_LR 0
#define DIGIT_TWO 1
#define DIGIT_THREE 2
#define DIGIT_FOUR 3

// Helper macro to set a specific bit in the buffer
#define SET_BUFF_BIT(addr, bit, enabled) buffer[addr] = (buffer[addr] & (~(1 << (bit)))) | (((enabled & 1)) << (bit));

/**
 * Setup the LCD
 * This function is called when the deivce is initialised using the 'attach' function.
 * It sets up the LCD and clears it.
 */
void KAV_A3XX_RUDDER_LCD::begin()
{
    ht_rad_tcas.begin();
    ht_rad_tcas.sendCommand(HT1621::RC256K);
    ht_rad_tcas.sendCommand(HT1621::BIAS_THIRD_4_COM);
    ht_rad_tcas.sendCommand(HT1621::SYS_EN);
    ht_rad_tcas.sendCommand(HT1621::LCD_ON);
    // This clears the LCD
    for (uint8_t i = 0; i < ht_rad_tcas.MAX_ADDR; i++)
        ht_rad_tcas.write(i, 0);

    // Initialises the buffer to all 0's.
    memset(buffer, 0, BUFFER_SIZE_MAX);
}

/**
 * Initialise the LCD (Entry Point)
 * This function is called to initialise the LCD.
 * @param CS The Chip Select pin
 * @param CLK The Clock pin
 * @param DATA The Data pin
 */
void KAV_A3XX_RUDDER_LCD::attach(byte CS, byte CLK, byte DATA)
{
    _CS = CS;
    _CLK = CLK;
    _DATA = DATA;
    _initialised = true;
    begin();
}

/**
 * Detach the LCD
 * Required for MobiFLight
 */
void KAV_A3XX_RUDDER_LCD::detach()
{
    if (!_initialised)
        return;
    _initialised = false;
}

/**
 * Refresh the LCD
 * After a change is made to a segment, the display must be refreshed so that
 * the change is visible.
 * @param address The address to refresh
 */
void KAV_A3XX_RUDDER_LCD::refreshLCD(uint8_t address)
{
    ht_rad_tcas.write(address * 2, buffer[address], 8);
}

/**
 * Clear the LCD
 * This function clears the LCD and resets the buffer.
 */
void KAV_A3XX_RUDDER_LCD::clearLCD()
{
    for (uint8_t i = 0; i < ht_rad_tcas.MAX_ADDR; i++)
        ht_rad_tcas.write(i, 0);
    memset(buffer, 0, BUFFER_SIZE_MAX);
}

void KAV_A3XX_RUDDER_LCD::clearDigit(uint8_t address)
{
    ht_rad_tcas.write(address * 2, 0);
    buffer[address] = 0;
}

// 'L', 'R' and Dot Functions
/**
 * Set the first character to show 'L' or blank.
 * @param enabled Whether to show 'L' (true) or blank (false)
 */
void KAV_A3XX_RUDDER_LCD::setLeft(bool enabled)
{
    if (enabled) {
        displayDigit(DIGIT_LR, 11);
    }
}

/**
 * Set the first character to show 'R' or blank.
 * @param enabled Whether to show 'R' (true) or blank (false)
 */
void KAV_A3XX_RUDDER_LCD::setRight(bool enabled)
{
    if (enabled) {
        displayDigit(DIGIT_LR, 12);
    }
}

/**
 * Set the dot to show or hide.
 * @param enabled Whether to show the dot (true) or hide it (false)
 */
void KAV_A3XX_RUDDER_LCD::setDot(bool enabled)
{
    // The dot is the 4th bit of the DIGIT_THREE address.
    SET_BUFF_BIT(DIGIT_THREE, 4, enabled);
    refreshLCD(DIGIT_THREE);
}

/**
 * Set the value of the LCD using an integer.
 * @param value The value to display
 */
void KAV_A3XX_RUDDER_LCD::setValueInt(int16_t value)
{
    // Convert the number from negative to positive if it's negative.
    if (value < 0) {
        value = value * -1;
    }

    // Display the value.
    if (value > 999)
        value = 999;
    displayDigit(DIGIT_FOUR, (value % 10));
    if (value > 9)
    {
        value = value / 10;
        displayDigit(DIGIT_THREE, (value % 10));
    }
    else
    {
        displayDigit(DIGIT_THREE, 0);
    }
    if (value > 9)
    {
        displayDigit(DIGIT_TWO, (value / 10));
    }
    else
    {
        displayDigit(DIGIT_TWO, 13);
    }

    setDot(true);
}

// Show values as a combined function
/**
 * Show the value on the display with the 'L' character enabled using an integer.
 * @param value The value to display
 */
void KAV_A3XX_RUDDER_LCD::showLeftValueInt(uint16_t value)
{
    setValueInt(value);
    setLeft(true);
}

/**
 * Show the value on the display with the 'R' character enabled using an integer.
 * @param value The value to display
 */
void KAV_A3XX_RUDDER_LCD::showRightValueInt(uint16_t value)
{
    setValueInt(value);
    setRight(true);
}

/**
 * Show the value on the display with the 'L' or 'R' character enabled using an integer.
 * @param value The value to display
*/
void KAV_A3XX_RUDDER_LCD::showLandRValue(int16_t value)
{
    if (value < 0)
    {
        setLeft(true);
        // Convert a negative value to a positive one to display it.
        value = value * -1;
    }
    else
    {
        setRight(true);
    }
    setValueInt(value);
}

// Global Functions
/**
 * A list of the binary patterns to show different characters on the LCD.
 */
uint8_t digitPatternRudder[14] = {
    0b11101011, // 0
    0b01100000, // 1
    0b11000111, // 2
    0b11100101, // 3
    0b01101100, // 4
    0b10101101, // 5 or S
    0b10101111, // 6
    0b11100000, // 7
    0b11101111, // 8
    0b11101101, // 9
    0b00000100, // -
    0b00001011, // L
    0b11111110, // R
    0b00000000, // blank
};

/**
 * Display a digit on a specific address.
 * @param address The address to display the digit on
 * @param digit The digit to display
 * @see digitPatternRudder
 */
void KAV_A3XX_RUDDER_LCD::displayDigit(uint8_t address, uint8_t digit)
{
    // This ensures that anything over 13 is turned to 'blank', and as it's unsigned, anything less than 0 will become 255, and therefore, 'blank'.
    if (digit > 13)
        digit = 13;

    buffer[address] = digitPatternRudder[digit];

    refreshLCD(address);
}

/**
 * Handle MobiFlight Commands
 * This function shouldn't be called be a user, it should only be called by the
 * custom device function. This is where data from MobiFlight enters the
 * library and is handled to be displayed on the LCD.
 * @param cmd The command from MobiFlight
 * @see handleMobiFlightRaw
 */
void KAV_A3XX_RUDDER_LCD::set(int16_t messageID, char *setPoint)
{
    // Using `strtol()` instead of `atoi()` to allow for larger value numbers.
    int32_t data = strtol(setPoint, NULL, 10);
    /* **********************************************************************************
        Each messageID has it's own value
        check for the messageID and define what to do.
        Important Remark!
        MessageID == -2 will be send from the board when PowerSavingMode is set
            Message will be "0" for leaving and "1" for entering PowerSavingMode
        MessageID == -1 will be send from the connector when Connector stops running
        Put in your code to enter this mode (e.g. clear a display)
    ********************************************************************************** */
    if (messageID == -1)
        return; // Ignore for now, handle this condition later.
    else if (messageID == -2)
        return; // Ignore for now, handle this condition later.
    else if (messageID == 0)
        setLeft((uint16_t)data);
    else if (messageID == 1)
        setRight((uint16_t)data);
    else if (messageID == 2)
        setDot((uint16_t)data);
    else if (messageID == 3)
        // This one needs to keep it's sign, so using `int16_t`.
        setValueInt((int16_t)data);
    else if (messageID == 4)
        showLeftValueInt((uint16_t)data);
    else if (messageID == 5)
        showRightValueInt((uint16_t)data);
    else if (messageID == 6)
        // This one needs to keep it's sign, so using `int16_t`.
        showLandRValue((int16_t)data);
}
