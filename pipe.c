#include "pipe.h"

#include <stddef.h>

pipe_error_t pipe_configure_length(pipe_t *pipe, const uint32_t len)
{
    // Check the pipe is not null
    if (pipe == NULL)
    {
        return PIPE_INVALID_PARAM;
    }

    pipe->length = len;

    return PIPE_OK;
}