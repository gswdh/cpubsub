#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {PIPE_WAIT_BLOCK, PIPE_WAIT_POLL} pipe_wait_t;

typedef struct
{

} pipe_t;

void pipe_init(pipe_t * pipe, uint32_t len);
void pipe_delete(pipe_t * pipe);
void pipe_push(pipe_t * pipe, void * p_item);
bool pipe_pop(pipe_t * pipe, void * p_item);

#endif