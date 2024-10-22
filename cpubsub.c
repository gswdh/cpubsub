#include "cpubsub.h"

#include <stddef.h>
#include <stdlib.h>

#include "messages.h"

static cps_node_t cps_node = {0};

cps_result_t cps_subscribe(topic_t topic, uint32_t topic_size, pipe_t *pipe)
{
    // Catch zero length pipe allocations
    if (pipe->length == 0)
    {
        pipe->length = CPS_PIPE_LENGTH_DEFAULT;
    }

    // Init the pipe
    pipe_init(pipe, topic_size, pipe->length);

    // First node in the list is not used
    cps_node_t *node = &cps_node;

    // Get to the end of the list
    while (node->next != NULL)
    {
        node = node->next;
    }

    // Make a new node on the end
    node->next = malloc(sizeof(cps_node_t));
    if (node->next == NULL)
    {
        return CPS_ALLOC_ERROR;
    }
    node = node->next;

    // Make the new node
    node->next  = NULL;
    node->pipe  = pipe;
    node->topic = topic;

    return CPS_OK;
}

cps_result_t cps_publish(void *msg, uint32_t mid)
{
    return cps_publish_ex(msg, mid, CPS_SRC_NORMAL);
}

cps_result_t cps_publish_ex(void *msg, uint32_t mid, cps_pub_src_t src)
{
    // Set the MID in the message
    ((cps_packet_template_t *)msg)->mid = mid;

    // Get the topic from the message type
    topic_t topic = ((cps_packet_template_t *)msg)->mid;

    // First node in the list is not used
    cps_node_t *node = &cps_node;

    // Get to the end of the list
    while (node->next)
    {
        // Advance
        node = node->next;

        // Publish if there's a match
        if ((node->topic == topic) || ((node->topic == CPS_NETWORK_MID) && (src == CPS_SRC_NORMAL)))
        {
            pipe_push(node->pipe, msg);
        }
    }

    return CPS_OK;
}

cps_result_t cps_publish_network(void *msg)
{
    // Get the topic from the message type
    topic_t topic = ((cps_packet_template_t *)msg)->mid;

    // First node in the list is not used
    cps_node_t *node = &cps_node;

    // Get to the end of the list
    while (node->next != NULL)
    {
        // Advance
        node = node->next;

        // Publish if there's a match
        if (node->topic == topic)
        {
            pipe_push(node->pipe, msg);
        }
    }

    return CPS_OK;
}

cps_result_t cps_receive(pipe_t *pipe, void *msg, pipe_wait_t wait)
{
    if (wait == PIPE_WAIT_BLOCK)
    {
        // Wait for a message
        while (pipe_pop(pipe, msg) == false)
        {
            cps_delay_ms(CPS_RX_BLOCK_INT_MS);
        }

        return CPS_OK;
    }

    else if (wait == PIPE_WAIT_POLL)
    {
        // Try for a message
        if (pipe_pop(pipe, msg) == true)
        {
            return CPS_OK;
        }
    }

    return CPS_NO_MSG;
}

topic_t cps_get_mid(void *data) { return ((cps_packet_template_t *)data)->mid; }

void __attribute__((weak)) cps_delay_ms(const uint32_t time_ms) { return; };