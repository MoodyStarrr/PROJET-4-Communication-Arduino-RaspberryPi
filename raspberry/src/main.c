#include "protocol.h"
#include "crc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
  uint8_t buffer[11]; // Pour Stocker sans padding ( de maniere contigu ) le message envoyée à la fonction crc

  frame_t message = {
    .start = FRAME_START,
    .command = CMD_SENSOR_READ,
    .length = 5,
    .payload = {0xA6,0x12,0x02,0x10,0x14,0x00,0x00,0x00},
    .end = FRAME_STOP,
  }; // Trame de message de test

  buffer[0] = message.command;
  buffer[1] = message.length;
  memcpy(buffer + 2, message.payload, 8);

  message.crc = crc8(buffer,10);

  printf("Voici la trame du message:\n");
  printf("start = %02X\n",message.start);
  printf("command = %02X\n",message.command);
  printf("length = %02X\n",message.length);

  printf("payload =\t");
  for(size_t i = 2 ; i < sizeof(buffer)/sizeof(uint8_t) - 1; i++){
    printf("%02X\t",buffer[i]);
  }
  printf("\n");

  printf("crc = %02X\n",message.crc);
  printf("end = %02X\n",message.end);

  return EXIT_SUCCESS;
}
