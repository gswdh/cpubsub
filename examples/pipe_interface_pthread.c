#include "pipe.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

pipe_error_t pipe_init(pipe_t *pipe, const uint32_t msg_size, const uint32_t queue_len)
{
    if (msg_size > PIPE_MAX_MSG_SIZE)
    {
        return PIPE_INVALID_PARAM;
    }

    // Ensure the mutex is initialized if it's NULL
    if (pipe->queue_lock == NULL)
    {
        pipe->queue_lock = malloc(sizeof(pthread_mutex_t));
        if (pipe->queue_lock == NULL)
        {
            return PIPE_ALLOC_ERROR;
        }
        pthread_mutex_init((pthread_mutex_t *)pipe->queue_lock, NULL);
    }

    // Lock mutex to ensure thread-safety while modifying the pipe
    pthread_mutex_lock((pthread_mutex_t *)pipe->queue_lock);

    // If the pipe already has a queue and the object size differs, we need to replace it
    if ((pipe->queue != NULL) && (pipe->msg_size != msg_size))
    {
        free(pipe->queue);  // Free the old queue
        pipe->queue = NULL; // Set the queue pointer to NULL
    }

    // Create the queue if it doesn't exist
    if (pipe->queue == NULL)
    {
        pipe->queue = malloc(queue_len * msg_size);
        if (pipe->queue == NULL)
        {
            pthread_mutex_unlock((pthread_mutex_t *)pipe->queue_lock); // Unlock before returning
            return PIPE_ALLOC_ERROR;
        }
        pipe->msg_size = msg_size;
        pipe->length   = queue_len;
    }

    pthread_mutex_unlock((pthread_mutex_t *)pipe->queue_lock); // Unlock after modification
    return PIPE_OK;
}

pipe_error_t pipe_delete(pipe_t *pipe)
{
    if (pipe->queue != NULL)
    {
        free(pipe->queue);  // Free the allocated queue memory
        pipe->queue = NULL; // Reset the queue pointer
    }

    // Destroy the mutex if it exists
    if (pipe->queue_lock != NULL)
    {
        pthread_mutex_destroy((pthread_mutex_t *)pipe->queue_lock);
        free(pipe->queue_lock); // Free the mutex memory
        pipe->queue_lock = NULL;
    }

    return PIPE_OK;
}

pipe_error_t pipe_push(const pipe_t *pipe, const void *p_item)
{
    // Ensure the pipe and item are valid
    if (pipe == NULL || p_item == NULL || pipe->queue == NULL)
    {
        return PIPE_PUSH_ERROR;
    }

    // Lock mutex to ensure thread-safety while modifying the pipe
    pthread_mutex_lock((pthread_mutex_t *)pipe->queue_lock);

    // Check if the queue has space (simplified for this example, assuming the queue is always full)
    // For a real implementation, you should track the number of elements in the queue and implement
    // overflow protection.

    // Copy the item into the queue (simple approach for this example)
    memcpy(pipe->queue, p_item, pipe->msg_size);

    pthread_mutex_unlock((pthread_mutex_t *)pipe->queue_lock); // Unlock after modification
    return PIPE_OK;
}

static uint8_t pipe_pop_msg_mem[PIPE_MAX_MSG_SIZE] = {0};

pipe_error_t pipe_pop(pipe_t *pipe, void *p_item, const uint32_t msg_size)
{
    // Ensure the pipe and item are valid, and check if object sizes match
    if (pipe == NULL || p_item == NULL || pipe->queue == NULL || msg_size != pipe->msg_size)
    {
        return PIPE_POP_ERROR;
    }

    // Lock mutex to ensure thread-safety while accessing the queue
    pthread_mutex_lock((pthread_mutex_t *)pipe->queue_lock);

    // Check if the queue is empty or an error occurs (simplified for this example)
    if (pipe->length <= 0)
    {
        pthread_mutex_unlock((pthread_mutex_t *)pipe->queue_lock);
        return PIPE_POP_ERROR;
    }

    // Copy the item from the queue to the local memory (simplified approach)
    memcpy(p_item, pipe->queue, msg_size);

    pthread_mutex_unlock((pthread_mutex_t *)pipe->queue_lock); // Unlock after modification
    return PIPE_OK;
}
