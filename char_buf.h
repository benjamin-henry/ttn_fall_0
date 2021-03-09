#ifndef CHAR_BUF_H
#define CHAR_BUF_H

#include "stdlib.h"

#ifndef __cplusplus
extern "C" {
#endif

typedef struct char_buf_t {
  unsigned char *data;
  unsigned char cnt;
  unsigned char max;
};
typedef char_buf_t* char_buf;

char_buf char_buf_init(unsigned char max);
void char_buf_reset(char_buf cbuf);
signed int char_buf_add(char_buf cbuf, unsigned char * c);
void char_buf_append(char_buf cbuf, unsigned char * data, unsigned char data_len);

//void loc2payload(char_buf signal, unsigned char * payload, int start);

#ifndef __cplusplus
}
#endif

#endif
