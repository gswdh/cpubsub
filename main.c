#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "pubsub.h"

void pipe_init(pipe_t * pipe, uint32_t len)
{
    // Create the FreeRTOS queue
    QueueHandle_t * xQueue = malloc(sizeof(xQueue));
    xQueueCreate(len, sizeof(uint32_t));

    // Assign the xQueue pointer
    pipe->queue = (void*)xQueue;
}

void pipe_delete(pipe_t * pipe)
{
    if(pipe->queue)
    {
        free(pipe->queue);
    }
    
    if(pipe)
    {
        free(pipe);
    }
}

void pipe_push(pipe_t * pipe, void * p_item)
{
    xQueueSend((QueueHandle_t)*pipe->queue, p_item, (TickType_t)0);
}

bool pipe_pop(pipe_t * pipe, void * p_item)
{
    if(xQueueReceive((QueueHandle_t)*pipe->queue, p_item, (TickType_t)10) == pdPASS)
      {
        return true;
      }

      return false;
}

int main(void)
{



    return 0;
}