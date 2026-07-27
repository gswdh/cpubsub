#include "cpubsub.h"

#include "cpubsub_config.h"

#include "sublist.h"

#include <stddef.h>
#include <stdlib.h>
#ifdef CPS_HEADER_USE_TIME_H
#include <time.h>
#endif

/**
 * @brief Retrieves the message ID from a given message following the standard message format.
 *
 * This function extracts the message ID from the header of the provided message.
 *
 * @param msg Pointer to the message structure.
 * @return The message ID (msg_id) of the provided message.
 */
static uint32_t cps_get_mid(const void *msg)
{
    // Check the message is not null
    if (msg == NULL)
    {
        return 0U;
    }

    return ((cps_packet_template_t *)msg)->header.msg_id;
}

cps_result_t cps_subscribe(pipe_t *pipe, const uint32_t msg_id, const uint32_t msg_size)
{
    // Check the pipe is not null
    if (pipe == NULL)
    {
        return CPS_INVALID_PARAM;
    }

    // Catch zero length pipe allocations
    if (pipe->length == 0U)
    {
        pipe->length = CPS_PIPE_LENGTH_DEFAULT;
    }

    // Init the pipe
    if (pipe_init(pipe, msg_size, pipe->length) != PIPE_OK)
    {
        return CPS_PIPE_INIT_ERROR;
    }

    // Add the subscription
    if (sublist_add_sub((const pipe_t *)pipe, (const uint32_t)msg_id) != SUBLIST_OK)
    {
        return CPS_SUB_ERROR;
    }

    return CPS_OK;
}

void cps_init_msg_header(void *msg, const uint32_t device_id, const uint32_t msg_id)
{
    // Check the message is not null
    if (msg == NULL)
    {
        return;
    }

    cps_packet_header_t *header = &((cps_packet_template_t *)msg)->header;

    header->msg_id    = msg_id;
    header->sender_id = device_id;

#ifdef CPS_HEADER_USE_TIMESTAMP
#ifdef CPS_HEADER_USE_TIME_H
    clock_t now                = clock();
    header->time.seconds_s     = (uint32_t)now / (uint32_t)1e6;
    header->time.subseconds_us = (uint32_t)now % (uint32_t)1e6;
#else
    header->time = cps_get_time();
#endif
#endif
}

cps_result_t cps_publish(const void *msg)
{
    // Check the message is not null
    if (msg == NULL)
    {
        return CPS_INVALID_PARAM;
    }

    // Get the msg_id from the packet
    const uint32_t msg_id = cps_get_mid(msg);

    // Get all the pipes that are subbed to this msg_id
    pipe_t        *pipes[CPS_MAX_SUBS] = {0};
    const uint32_t n_subs              = sublist_get_subs(pipes, msg_id, CPS_MAX_SUBS);

    // Get to the end of the list
    for (uint32_t i = 0U; i < n_subs; i++)
    {
        if (pipe_push(pipes[i], msg) != PIPE_OK)
        {
            return CPS_PUB_ERROR;
        }
    }

    return CPS_OK;
}

#ifdef CPS_HEADER_USE_NETWORKING
cps_result_t cps_publish_from_network(const void *msg, const uint32_t device_id)
{
    // Check the message is not null
    if (msg == NULL)
    {
        return CPS_INVALID_PARAM;
    }

    const uint32_t sender_id = ((cps_packet_template_t *)msg)->header.sender_id;

    // If the device IDs match it means the message was orignally from us...
    if (device_id == sender_id)
    {
        // ...so do not publish it
        return CPS_PUB_ERROR;
    }

    return cps_publish(msg);
}
#endif

cps_result_t cps_receive(const pipe_t *pipe, void *msg, const cps_wait_t wait)
{
    // Check the pipe is not null
    if (pipe == NULL || msg == NULL)
    {
        return CPS_INVALID_PARAM;
    }

    if (wait == CPS_WAIT_BLOCK)
    {
        // Wait here until we get a message
        while (pipe_pop(pipe, msg, (const uint32_t)pipe->msg_size) != PIPE_OK)
        {
            cps_delay_ms(CPS_RX_BLOCK_INT_MS);
        }

        return CPS_OK;
    }

    else if (wait == CPS_WAIT_POLL)
    {
        // Try once for a message
        if (pipe_pop(pipe, msg, (const uint32_t)pipe->msg_size) == PIPE_OK)
        {
            return CPS_OK;
        }
    }

    else
    {
        return CPS_INVALID_PARAM;
    }

    return CPS_NO_MSG;
}
