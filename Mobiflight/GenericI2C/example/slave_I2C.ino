#include <Arduino.h>
#include "Wire.h"

#define I2C_MOBIFLIGHT_ADDR             0x27

#define END_OF_I2C_MESSAGE              0x00
#define END_OF_I2C_COMMAND              0x0D      // carriage return in ASCII
#define END_OF_I2C_PARTIAL_MESSAGE      0x0A      // line feed in ASCII
#define MAX_LENGTH_MESSAGE              80        // max length of a message is 80 bytes due to limitations of the CMDmessenger


enum {
    NOT_SYNCHRONIZED = 0,
    RECEIVE_COMMAND,
    RECEIVE_DATA
};

void onReceiveI2C(int len);

int16_t messageID = 0;
char message[MAX_LENGTH_MESSAGE];
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
  char buffer[7] = {0};                                    // range is -32768 ... 32767 -> max. 6 character plus terminating NULL

  for (uint8_t i = 0; i < received_bytes; i++) {
    switch (state) {
    case NOT_SYNCHRONIZED:
      if (Wire.read() == END_OF_I2C_MESSAGE) {              // wait for end of message to get synchronized
        state = RECEIVE_COMMAND;
        byte_counter = 0;
      }
      break;

    case RECEIVE_COMMAND:
      buffer[i] = Wire.read();
      if (buffer[i] == END_OF_I2C_COMMAND) {
        buffer[i] = 0x00;                                   // terminate string
        messageID = atoi(buffer);
        state = RECEIVE_DATA;                               // next bytes are Data Bytes
      }
      if (i >= 6) {                                         // buffer overflow for messageID
        state == NOT_SYNCHRONIZED;                          // something went wrong, get a new synchronization
        byte_counter = 0;
        return;
      }
      break;

    case RECEIVE_DATA:
      message[byte_counter] = Wire.read();
      if (message[byte_counter] == END_OF_I2C_MESSAGE) {    // end of message detected, prepare for receiving next messageID
        byte_counter = 0;
        state = RECEIVE_COMMAND;
        message_received = true;
        return;
      } else if(message[byte_counter] == END_OF_I2C_PARTIAL_MESSAGE) {   // end of partial message detected, next transmission will be rest of message  
        return;                                             // keep receiving data
      } else {
        byte_counter++;                                     // get the next byte
      }
      break;

    default:
      break;
    }
  }
  state = NOT_SYNCHRONIZED;                                 // We shouldn't come here, something went wrong
}
