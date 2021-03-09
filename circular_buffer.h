#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "stdlib.h"

#ifndef __cplusplus
extern "C" {
#endif


typedef struct circular_buf_t {
  float * buffer;
  int head;
  int tail;
  int vhead;
  int vtail;
  int max; //of the buffer
  bool full;
  bool vfull;
  bool locked;
};
typedef circular_buf_t* cbuf_handle_t;

// void circular_buf_init(cbuf_handle_t cbuff, float* buffer, size_t size);
cbuf_handle_t circular_buf_init(float* buffer, size_t size);
void circular_buf_Vinit(cbuf_handle_t cbuf);

void circular_buf_free(cbuf_handle_t cbuf);

void circular_buf_reset(cbuf_handle_t cbuf);

void circular_buf_put(cbuf_handle_t cbuf, float data);

int circular_buf_get(cbuf_handle_t cbuf, float * data);

bool circular_buf_empty(cbuf_handle_t cbuf);

bool circular_buf_full(cbuf_handle_t cbuf);

void shift_left(cbuf_handle_t cbuf, unsigned int n);

size_t circular_buf_capacity(cbuf_handle_t cbuf);

size_t circular_buf_size(cbuf_handle_t cbuf);

static void advance_pointer(cbuf_handle_t cbuf);
static void retreat_pointer(cbuf_handle_t cbuf);

void increase_vhead(cbuf_handle_t cbuf);
void advance_vpointer(cbuf_handle_t cbuf);
void retreat_vpointer(cbuf_handle_t cbuf);
void decrease_vtail(cbuf_handle_t cbuf);

#ifndef __cplusplus
}
#endif

#endif
