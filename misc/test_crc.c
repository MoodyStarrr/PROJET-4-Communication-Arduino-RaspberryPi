#include <stdio.h>
#include <stdlib.h>
#include "crc.h"

typedef struct {
  uint8_t value[10];
  size_t length;
  uint8_t result;
}CrC_Value;

int main(void){
  CrC_Value array[] = {
    {{0xB},1,0x20},
    {{0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39},9,0xA1},
    {{0xA6,0x12,0x02,0x10,0x14},5,0xCF},
    {.length = 0,.result = 0x00},
    {{0xA6,0x12,0x02,0x15,0x14},5,0x30},
  };
  int error = 0;

  for(int i = 0 ; i < (sizeof(array)/sizeof(CrC_Value)) ; i++){
    uint8_t MyResult = crc8(array[i].value,array[i].length);

    for(size_t j = 0 ; j < array[i].length ; j++){
      printf("\t%02X",array[i].value[j]);
    }

    printf(" is of %zu size and equals %X usualy and %X with my function.\n",array[i].length,array[i].result,MyResult);

    if(MyResult != array[i].result)
      error += 1;
  }

  if(error)
    exit(EXIT_FAILURE);
  exit(EXIT_SUCCESS);
}
