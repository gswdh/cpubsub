#ifndef _PUBSUB_H_
#define _PUBSUB_H_

#include <stdint.h>

#include "pipe.h"
#include "topic.h"

typedef enum
{
    PS_OK,
    PS_NO_MSG,
    PS_ALLOC_ERROR
} ps_result_t;

typedef struct ps_node_t
{
    struct ps_node_t *next;
    topic_t           topic;
    pipe_t *          pipe;
} ps_node_t;

typedef struct
{
	uint32_t mid;
    void * ptr;
} cPS_Packet_Template_t;

ps_result_t ps_subscribe(topic_t topic, pipe_t *pipe);
ps_result_t ps_publish(void *msg);
ps_result_t ps_receive(pipe_t *pipe, void *msg, pipe_wait_t wait);
topic_t cPS_get_mid(void * data);

#endif