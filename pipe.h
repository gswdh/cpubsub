#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    PIPE_WAIT_BLOCK,
    PIPE_WAIT_POLL
} pipe_wait_t;

typedef struct
{
    void *   queue;
    uint32_t obj_size;
} pipe_t;

void pipe_init(pipe_t *pipe, uint32_t obj_size, uint32_t queue_len);
void pipe_delete(pipe_t *pipe);
void pipe_push(pipe_t *pipe, void *p_item);
bool pipe_pop(pipe_t *pipe, void *p_item);

#endif