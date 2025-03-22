#ifndef _SUBLIST_H_
#define _SUBLIST_H_

#include "cpubsub_config.h"

#include "pipe.h"

#include <stdint.h>

typedef enum
{
    SUBLIST_OK = 0U,
    SUBLIST_NO_SPACE,
    SUBLIST_INVALID_PARAM,
} sublist_result_t;

typedef enum
{
    SUBLIST_NO_SUB = 0U,
    SUBLIST_SUBBED,
} sublist_status_t;

typedef struct
{
    sublist_status_t status;
    uint32_t         msg_id;
    pipe_t          *pipe;
} sublist_subs_t;

/**
 * @brief Adds a subscription for a specific pipe and message ID.
 *
 * This function adds a subscription to the subscription list by associating a pipe
 * with a specific message ID. It finds an available slot in the subscription list
 * and marks it as "subscribed."
 *
 * @param pipe Pointer to the pipe to be subscribed.
 * @param msg_id The message ID that the pipe is subscribing to.
 * @return SUBLIST_OK if the subscription was successfully added, SUBLIST_NO_SPACE if no available
 * space.
 */
sublist_result_t sublist_add_sub(const pipe_t *pipe, const uint32_t msg_id);

/**
 * @brief Removes a subscription for a specific pipe and message ID.
 *
 * This function removes the subscription that matches the provided pipe and message ID.
 * It marks the subscription as "not subscribed."
 *
 * @param pipe Pointer to the pipe to unsubscribe.
 * @param msg_id The message ID to remove the subscription for.
 */
void sublist_remove_sub(const pipe_t *pipe, const uint32_t msg_id);

/**
 * @brief Retrieves the pipes that are subscribed to a specific message ID.
 *
 * This function fills the provided pipes array with all the pipes that are subscribed
 * to a given message ID. It returns the number of pipes found, up to a specified limit.
 *
 * @param pipes Array of pointers to store the pipes that are subscribed to the message ID.
 * @param msg_id The message ID to search for.
 * @param max_pipes The maximum number of pipes to return.
 * @return The number of pipes found that are subscribed to the message ID.
 */
uint32_t sublist_get_subs(pipe_t **pipes, const uint32_t msg_id, const uint32_t max_pipes);

#endif