#ifndef FRAMER_H
#define FRAMER_H

#define _POSIX_C_SOURCE 200809L

#include "protocol.h"
#include <stdbool.h>
#include <pthread.h>
// Pas besoin d'include stdint deja fait dans protocol.h

typedef enum {
  WAIT_START,
  READ_COMMAND,
  READ_LENGTH,
  READ_PAYLOAD,
  READ_CRC,
  READ_END,
}framer_internal_state_t;

typedef struct {
  framer_internal_state_t state;
  frame_t current_message;
  bool escape; // Utile pour le Byte Stuffing
  uint8_t payload_index;
}framer_context_t;

typedef struct {
  bool frame_ready;
  pthread_cond_t ready;
  pthread_mutex_t mutex;
}framer_thread_state_t;

typedef enum {
  FRAME_IN_PROGRESS,
  FRAME_VALID,
  FRAME_INVALID,
}framer_return_t;

framer_return_t frame_parser(uint8_t received, framer_context_t * ctx);

void framer_reset(framer_context_t * ctx);

#endif
