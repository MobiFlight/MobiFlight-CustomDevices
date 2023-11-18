#include <Arduino.h>
#include "Wire.h"

#define I2C_MOBIFLIGHT_ADDR     0x27
//#define BUFFER_LENGTH         I2C_BUFFER_LENGTH   // uncomment this for ESP32
//#define BUFFER_LENGTH         WIRE_BUFFER_SIZE    // uncomment this for Raspberry Pico

enum {
    NOT_SYNCHRONIZED = 0,
    RECEIVE_COMMAND,
    RECEIVE_DATA
};

void onReceiveI2C(int len);

int16_t messageID = 0;
char message[BUFFER_LENGTH];
bool message_received = false;


void setup() {
  Serial.begin(115200);

  // setup i2C receive callback
  Wire.onReceive(onReceiveI2C);

  // init I2C busses
  Wire.begin(I2C_MOBIFLIGHT_ADDR);
  Wire.setClock(400000);
}

void loop() {

  if (message_received) {
    message_received = false;
    switch (messageID)
    {
    case -1:
      // received messageID is 0
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is -1 and Payload is: "); Serial.println(message);
      break;

    case 0:
      // received messageID is 0
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is 0 and Payload is: "); Serial.println(message);
      break;

    case 1:
      // received messageID is 1
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is 1 and Payload is: "); Serial.println(message);
      break;

    case 2:
      // received messageID is 2
      // data is a string in message[] and 0x00 terminated
      // do something with your received data
      Serial.print("MessageID is 2 and Payload is: "); Serial.println(message);
      break;
    
    default:
      break;
    }
  }
}


// callback function for I2C receive
void onReceiveI2C(int received_bytes) { 

static uint8_t byte_counter = 0;
static uint8_t state = RECEIVE_COMMAND;
char buffer[7] = {0};                                 // range is -32768 ... 32767 -> max. 6 character plus terminating NULL

  if (state == NOT_SYNCHRONIZED) {
    for (uint8_t i = 0; i < received_bytes; i++) {
      if (Wire.read() == 0) {                         // wait for 0x00 to get synchronized
        state = RECEIVE_COMMAND;
        break;
      }
      byte_counter++;
    }
    received_bytes -= byte_counter;
    byte_counter = 0;
  }

  for (uint8_t i = 0; i < received_bytes; i++) {
    if (state == RECEIVE_COMMAND) {                    // first Bytes of message is the messageID
        buffer[i] = Wire.read();
      if (buffer[i] == 0x0D) {
        buffer[i] = 0x00;
        messageID = atoi(buffer);
        state = RECEIVE_DATA;                         // next bytes are Data Bytes
      }      
    } else if(state == RECEIVE_DATA) {
        message[byte_counter] = Wire.read();
        if (message[byte_counter] == 0) {             // end of message detected, prepare for receiving next messageID
          byte_counter = 0;
          state = RECEIVE_COMMAND;
          message_received = true;
          return;
        } else {
          byte_counter++;
      }
    }
  }
}
