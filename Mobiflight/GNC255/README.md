This custom device supports a 256x64 pixel OLED display with predifined outputs like the original GNC255.
The display uses hardware SPI for communication, so you MUST use the following pins:

Mega
* CLK: Pin 52
* Data: Pin 51
* CS: Pin 53 for the first display, for more choose as you want
* Pin 50 can NOT be used for any other device

Pico
* CLK: Pin 18
* Data: Pin 19
* CS: Pin 17 for the first display, for more choose as you want
* Pin 16 can NOT be used for any other device
For both boards D/C and Reset can be choosen as you want.

For now these pins have to be defined within the connector to mark them as used. This will be changed later.

Connect your display accordingly the above used pins. Depending on your display, you might need to adjust some 
jumpers on the controller board to enable the SPI interface - below is one example of such display that works
once the zero ohm resistors were moved to the positions marked with yellow. Some boards might be already configured
for SPI, so check the datasheet or sales listing carefully.

![image](https://github.com/MobiFlight/MobiFlight-CustomDevices/assets/2587818/c2368dea-6da4-47e7-84b9-33f550802159)

