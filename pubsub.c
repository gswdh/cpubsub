#include "pubsub.h"

#include <stddef.h>
#include <stdlib.h>

static ps_node_t ps_node = {0};

ps_result_t ps_subscribe(topic_t topic, pipe_t *pipe)
{
    // First node in the list is not used
    ps_node_t *node = &ps_node;

    // Get to the end of the list
    while (node->next)
    {
        node = node->next;
    }

    // Make a new node on the end
    node->next = malloc(sizeof(ps_node_t));
    if (node->next == NULL)
    {
        return PS_ALLOC_ERROR;
    }
    node = node->next;

    // Make the new node
    node->next  = 0;
    node->pipe  = pipe;
    node->topic = topic;

    return PS_OK;
}

ps_result_t ps_publish(topic_t topic, void *msg)
{
    // First node in the list is not used
    ps_node_t *node = &ps_node;

    // Get to the end of the list
    while (node->next)
    {
        // Advance
        node = node->next;

        // Publish if there's a match
        if (node->topic == topic)
        {
            pipe_push(node->pipe, msg);
        }
    }

    return PS_OK;
}

ps_result_t ps_receive(pipe_t *pipe, void *msg, pipe_wait_t wait)
{
    if (wait == PIPE_WAIT_BLOCK)
    {
        // Wait for a message
        while (!pipe_pop(pipe, msg))
        {
        }

        return PS_OK;
    }

    if (wait == PIPE_WAIT_POLL)
    {
        // Try for a message
        if (pipe_pop(pipe, msg))
        {
            return PS_OK;
        }
    }

    return PS_NO_MSG;
}
