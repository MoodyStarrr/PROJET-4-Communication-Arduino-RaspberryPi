#include "framer.h"
#include <string.h>

void framer_reset(framer_context_t * ctx){
  ctx->escape = 0;
  ctx->payload_index = 0;
  ctx->state = WAIT_START;
  memset(&(ctx->current_message),0,sizeof(ctx->current_message));
}

static bool byte_stuffing_check(uint8_t received, framer_context_t * ctx){
  if(received == FRAME_ESCAPE && ctx->escape == 0){
    ctx->escape = 1;
    return false;
  }
  ctx->escape = 0;
  return true;
}

framer_return_t frame_parser(uint8_t received, framer_context_t * ctx){
  switch (ctx->state)
  {
    case WAIT_START :
      if(received == FRAME_START){
        ctx->state = READ_COMMAND;
      }
      return FRAME_IN_PROGRESS;

    case READ_COMMAND :
      if(byte_stuffing_check(received,ctx)){
        ctx->current_message.command = received;
        ctx->state = READ_LENGTH;
      }
      return FRAME_IN_PROGRESS;

    case READ_LENGTH :
      if(byte_stuffing_check(received,ctx)){
        if(received > SIZE_MAX_PAYLOAD){
          framer_reset(ctx);
          return FRAME_INVALID;
        }else{
          ctx->current_message.length = received;
          ctx->state = READ_PAYLOAD;
        }
      }
      return FRAME_IN_PROGRESS;

    case READ_PAYLOAD :
      if(byte_stuffing_check(received,ctx)){
        if(ctx->payload_index < ctx->current_message.length ){
          ctx->current_message.payload[ctx->payload_index++] = received;
          ctx->state = READ_PAYLOAD;
        }

        if(ctx->payload_index == ctx->current_message.length)
          ctx->state = READ_CRC;

      }
      return FRAME_IN_PROGRESS;

    case READ_CRC:
      if(byte_stuffing_check(received,ctx)){
        ctx->current_message.crc = received;

        uint8_t buffer[10]; // command + length + payload
        buffer[0] = ctx->current_message.command;
        buffer[1] = ctx->current_message.length;

        memcpy(buffer + 2, ctx->current_message.payload, 8);

        if( crc8(buffer,10) != ctx->current_message.crc ){
          framer_reset(ctx);
          return FRAME_INVALID;
        }
      }
      ctx->state = READ_END;
      return FRAME_IN_PROGRESS;

    case READ_END:
      if( received == FRAME_STOP ){
        pthread_cond_signal(ctx->ready); // Besoin de mutex ? ou le thread de reception fera un lock qui empechera le reset dans la ligne suivante ?
        framer_reset(ctx);
        ctx->state = WAIT_START;
        return FRAME_VALID;// Si la fonction framer dit que le message est pret + reset + state au bon endroit à quoi sert le retour frame valid ?
      }
      framer_reset(ctx);
      ctx->state = WAIT_START;
      return FRAME_INVALID;

  }
}


