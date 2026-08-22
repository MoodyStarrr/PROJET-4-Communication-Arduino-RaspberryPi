#include "crc.h"

uint8_t crc8(const uint8_t * Message, size_t MessageLength){
  uint8_t crc = 0; // Init = 0x00

  for(size_t i = 0 ; i < MessageLength ; i++){
    crc ^= Message[i];

    for(int j = 0 ; j < 8 ; j++){ // 8 car il y a 8 bit dans 1 octet
      uint8_t Multi = crc & 1; // On regarde le premier bit

      if(Multi == 1)
        crc = (crc >> 1) ^ 0x8C; // Polynome reflechi de 0x31
      else
        crc = crc >> 1;

    }
  }

  return crc;
}
