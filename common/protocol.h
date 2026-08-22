#ifndef PROTOCOL_H // Header Guard
#define PROTOCOL_H

#include <stdint.h>
//#include <> // For potential later uses

#define SIZE_MAX_PAYLOAD 8 // Maximum size for payload 
#define SIZE_MAX_MSG 24 // Maximum size for what's in between start and end * 2 + start and end size
#define FRAME_START 0x7E // Start communication byte
#define FRAME_STOP 0xFF // End communication byte

typedef enum {
  CMD_SENSOR_READ,
  CMD_SET_LED,
  CMD_OUTPUT_WRITE,
}cmd; // Command Enumaration used in Message for instructions

typedef struct {
  uint8_t start; // 1 octet
  uint8_t command; // 1 octet
  uint8_t length;// 1 octet
  uint8_t payload[SIZE_MAX_PAYLOAD];
  uint8_t crc;// 1 octet
  uint8_t end;// 1 octet
}frame_t; // Message Structure

#endif
