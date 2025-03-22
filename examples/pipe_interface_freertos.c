#include "pipe.h"

#include <stdlib.h>

#include <FreeRTOS.h>
#include <queue.h>

pipe_error_t pipe_init(pipe_t *pipe, const uint32_t msg_size, const uint32_t queue_len)
{
    if (msg_size > PIPE_MAX_MSG_SIZE)
    {
        return PIPE_INVALID_PARAM;
    }

    // Do we already have a queue for this pipe?
    if ((pipe->queue != NULL) && (pipe->msg_size < msg_size))
    {
        // Destory this queue to make a new one
        vQueueDelete(*(QueueHandle_t *)(pipe->queue));
        free(pipe->queue);
        pipe->queue = NULL;
    }

    // Make a queue if needed
    if (pipe->queue == NULL)
    {
        // Create the FreeRTOS queue
        pipe->queue = malloc(sizeof(QueueHandle_t));
        if (pipe->queue == NULL)
        {
            return PIPE_ALLOC_ERROR;
        }
        *(QueueHandle_t *)pipe->queue = xQueueCreate(queue_len, msg_size);

        // Take note of the object size
        pipe->msg_size = msg_size;
    }

    return PIPE_OK;
}

pipe_error_t pipe_delete(pipe_t *pipe)
{
    if (pipe->queue != NULL)
    {
        vQueueDelete(pipe->queue);
    }

    return PIPE_OK;
}

pipe_error_t pipe_push(const pipe_t *pipe, const void *p_item)
{
    if (xQueueSend(*(QueueHandle_t *)pipe->queue, (void *)p_item, (TickType_t)0) != pdPASS)
    {
        return PIPE_PUSH_ERROR;
    }

    return PIPE_OK;
}

static uint8_t pipe_pop_msg_mem[PIPE_MAX_MSG_SIZE] = {0};

pipe_error_t pipe_pop(const pipe_t *pipe, void *p_item, const uint32_t msg_size)
{
    // Copy the message out of the queue into the local memory
    if (xQueueReceive(*(QueueHandle_t *)pipe->queue, (void *)pipe_pop_msg_mem, (TickType_t)10) !=
        pdPASS)
    {
        return PIPE_POP_ERROR;
    }

    // Copy from the local memory to the passed pointer of the correct length
    memcpy((void *)p_item, (void *)pipe_pop_msg_mem, (size_t)msg_size);

    return PIPE_OK;
}