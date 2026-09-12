#include "framer.h"
#include <string.h>

void framer_reset(framer_context_t * ctx){
  ctx->escape = 0;
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
  }

}
