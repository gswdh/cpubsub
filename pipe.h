#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdbool.h>
#include <stdint.h>

#include "cpubsub_config.h"

#ifndef CPS_MAX_MSG_SIZE
#define PIPE_MAX_MSG_SIZE (256U)
#endif

typedef enum
{
    PIPE_OK = 0U,
    PIPE_INVALID_PARAM,
    PIPE_PUSH_ERROR,
    PIPE_POP_ERROR,
    PIPE_ALLOC_ERROR,
} pipe_error_t;

typedef struct
{
    void *queue;
#ifdef CPS_HEADER_USE_QUEUE_LOCK
    void *queue_lock;
#endif
    uint32_t msg_size;
    uint32_t length;
} pipe_t;

/**
 * @brief Initializes a pipe with a specified object size and queue length.
 *
 * This function initializes the pipe by allocating and creating a queue for the specified
 * object size and queue length. If the pipe already has a queue, it will be deleted and replaced.
 *
 * @param pipe Pointer to the pipe to initialize.
 * @param msg_size The size of each object in the queue.
 * @param queue_len The length (number of elements) of the queue.
 * @param pipe_type The type of the pipe used to denote if this pipe is used for networking.
 * @return PIPE_OK if the pipe was successfully initialized, an error code otherwise.
 */
pipe_error_t pipe_init(pipe_t *pipe, const uint32_t msg_size, const uint32_t queue_len);

/**
 * @brief Deletes the queue associated with the pipe.
 *
 * This function deletes the queue associated with the pipe and frees the resources.
 *
 * @param pipe Pointer to the pipe to delete.
 * @return PIPE_OK if the pipe was successfully deleted, an error code otherwise.
 */
pipe_error_t pipe_delete(pipe_t *pipe);

/**
 * @brief Pushes an item onto the pipe's queue.
 *
 * This function pushes an item onto the pipe's queue. If the queue is full or the item cannot
 * be added for any reason, it will return an error.
 *
 * @param pipe Pointer to the pipe to push the item onto.
 * @param p_item Pointer to the item to push onto the queue.
 * @return PIPE_OK if the item was successfully pushed, PIPE_PUSH_ERROR if an error occurred.
 */
pipe_error_t pipe_push(const pipe_t *pipe, const void *p_item);

/**
 * @brief Pops an item from the pipe's queue and copies it to a specified local memory location.
 *
 * This function retrieves an item from the pipe's queue and copies it into the provided
 * memory location, ensuring the correct object size. If the item cannot be retrieved
 * or copied due to errors, it returns an error.
 *
 * @param pipe Pointer to the pipe to pop the item from.
 * @param p_item Pointer to the buffer where the popped item will be stored.
 * @param msg_size The size of the object to copy.
 * @return PIPE_OK if the item was successfully popped and copied, PIPE_POP_ERROR if an error
 * occurred.
 */
pipe_error_t pipe_pop(pipe_t *pipe, void *p_item, const uint32_t msg_size);

static inline void pipe_configure_length(pipe_t *pipe, const uint32_t len) { pipe->length = len; }

#endif