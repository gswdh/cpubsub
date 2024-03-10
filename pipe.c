#include "pipe.h"

#include <stddef.h>

void __attribute__((weak)) pipe_init(pipe_t *pipe, uint32_t obj_size, uint32_t queue_len) {}

void __attribute__((weak)) pipe_delete(pipe_t *pipe) {}

void __attribute__((weak)) pipe_push(pipe_t *pipe, void *p_item) {}

bool __attribute__((weak)) pipe_pop(pipe_t *pipe, void *p_item) { return false; }
