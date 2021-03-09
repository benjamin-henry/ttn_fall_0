#include "circular_buffer.h"
#include "stdlib.h"


cbuf_handle_t circular_buf_init(float* buffer, size_t size) {
  // assert(buffer && size);

  cbuf_handle_t cbuf = (cbuf_handle_t)malloc(sizeof(circular_buf_t));
  // assert(cbuf);

  cbuf->buffer = buffer;
  cbuf->max = size;
  cbuf->locked = false;
  circular_buf_reset(cbuf);

  // assert(circular_buf_empty(cbuf));

  return cbuf;
}

void circular_buf_Vinit(cbuf_handle_t cbuf) {
  cbuf->vhead = cbuf->head;
  cbuf->vtail = cbuf->tail;
  cbuf->vfull = cbuf->full;
}

void circular_buf_reset(cbuf_handle_t cbuf) {
  cbuf->head = 0;
  cbuf->tail = 0;
  cbuf->full = false;
  circular_buf_Vinit(cbuf);
}

void circular_buf_free(cbuf_handle_t cbuf) {
  free(cbuf);
}

bool circular_buf_full(cbuf_handle_t cbuf) {
  return cbuf->full;
}

size_t circular_buf_capacity(cbuf_handle_t cbuf) {
  return cbuf->max;
}

size_t circular_buf_size(cbuf_handle_t cbuf) {
  size_t size = cbuf->max;
  if (!cbuf->full)  {
    if (cbuf->head >= cbuf->tail) {
      size = (cbuf->head - cbuf->tail);
    }
    else {
      size = (cbuf->max + cbuf->head - cbuf->tail);
    }
  }
  return size;
}


static void advance_pointer(cbuf_handle_t cbuf) {
  if (cbuf->full) {
    if (++(cbuf->tail) == cbuf->max) {
      cbuf->tail = 0;
    }
  }

  if (++(cbuf->head) == cbuf->max) {
    cbuf->head = 0;
  }
  cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(cbuf_handle_t cbuf) {
  cbuf->full = false;
  if (++(cbuf->tail) == cbuf->max) {
    cbuf->tail = 0;
  }
}

void advance_vpointer(cbuf_handle_t cbuf) {
  if (cbuf->vfull) {
    if (++(cbuf->vtail) == cbuf->max) {
      cbuf->vtail = 0;
    }
  }

  if (++(cbuf->vhead) == cbuf->max) {
    cbuf->vhead = 0;
  }
  cbuf->vfull = (cbuf->vhead == cbuf->vtail);
}

void retreat_vpointer(cbuf_handle_t cbuf) {
  cbuf->vfull = false;
  if (++(cbuf->vtail) == cbuf->max) {
    cbuf->vtail = 0;
  }
}

void decrease_vtail(cbuf_handle_t cbuf) {
  if(--(cbuf->vtail) == -1) {
    cbuf->vtail = cbuf->max-1;
  }
}

void circular_buf_put(cbuf_handle_t cbuf, float data) {
  cbuf->buffer[cbuf->head] = data;
  advance_pointer(cbuf);
}

bool circular_buf_empty(cbuf_handle_t cbuf){
  return circular_buf_size(cbuf) == 0;
}

int circular_buf_get(cbuf_handle_t cbuf, float * data) {
  int r = -1;
  if (!circular_buf_empty(cbuf)) {
    *data = cbuf->buffer[cbuf->tail];
    retreat_pointer(cbuf);
    r = 0;
  }
  return r;
}


void shift_left(cbuf_handle_t cbuf, unsigned int n) {
  do{
    if (!circular_buf_empty(cbuf)) {
      retreat_pointer(cbuf);
    }
  }while(--n);
}
