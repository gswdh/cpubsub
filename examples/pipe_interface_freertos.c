#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "pubsub.h"

void pipe_init(pipe_t *pipe, uint32_t len)
{
    // Create the FreeRTOS queue
    pipe->queue          = malloc(sizeof(QueueHandle_t));
    QueueHandle_t xQueue = xQueueCreate(len, sizeof(void *));

    // Assign the xQueue pointer
    *(QueueHandle_t *)pipe->queue = xQueue;
}

void pipe_delete(pipe_t *pipe)
{
    if (pipe->queue)
    {
        free(pipe->queue);
    }

    if (pipe)
    {
        free(pipe);
    }
}

void pipe_push(pipe_t *pipe, void *p_item)
{
    xQueueSend(*(QueueHandle_t *)pipe->queue, (void *)p_item, (TickType_t)0);
}

bool pipe_pop(pipe_t *pipe, void *p_item)
{
    if (xQueueReceive(*(QueueHandle_t *)pipe->queue, (void *)p_item, (TickType_t)10) == pdPASS)
    {
        return true;
    }

    return false;
}
