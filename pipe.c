#include "pipe.h"

#include <assert.h>
#include <stddef.h>

void __attribute__((weak)) pipe_init(pipe_t *pipe, uint32_t obj_size, uint32_t queue_len)
{
    assert(false && "Function not implemented.");
}

void __attribute__((weak)) pipe_delete(pipe_t *pipe)
{
    assert(false && "Function not implemented.");
}

void __attribute__((weak)) pipe_push(pipe_t *pipe, void *p_item)
{
    assert(false && "Function not implemented.");
}

bool __attribute__((weak)) pipe_pop(pipe_t *pipe, void *p_item)
{
    assert(false && "Function not implemented.");
    return false;
}

uint32_t pipe_item_size(pipe_t *pipe) { return pipe->obj_size; }