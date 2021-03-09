#include "char_buf.h"
#include <stdlib.h>

char_buf char_buf_init(unsigned char max){
    char_buf cbuf = (char_buf)malloc(sizeof(char_buf_t));
    cbuf->max = max;
    cbuf->cnt = 0;
    cbuf->data = (unsigned char*)malloc(sizeof(unsigned char)*max);
    return cbuf;
}

void char_buf_reset(char_buf cbuf){
    do {
        cbuf->data[cbuf->cnt] = 0;
    }while((cbuf->cnt)--);
}

signed int char_buf_add(char_buf cbuf, unsigned char * c){
    if((cbuf->cnt)+1 <= cbuf->max) {
        cbuf->data[(cbuf->cnt)++] = *c;
        return 1;
    }
    else {
        return -1;
    }
}

void char_buf_append(char_buf cbuf, unsigned char * data, unsigned char data_len) {
    if(data_len+(cbuf->cnt) <= cbuf->max) {
        for(int i = 0; i< data_len;i++) {
            cbuf->data[(cbuf->cnt)++] = data[i];
        }
    }
}
