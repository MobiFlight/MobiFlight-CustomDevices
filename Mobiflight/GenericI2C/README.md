This device forwardes the defined messages from mobiflight.genericI2C.device.json to I2C.
If you need more than 2 messages, extend the device.json file accordingly.

Connect an I2C device to the 2wire bus to receive the informations. This device must handle the messageID and the message.
The messageID and the message is send as a string.

In the folder /example an .ino is available which shows how to receive messageID and message and how to handle this.
