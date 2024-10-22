#ifndef _CPUBSUB_H_
#define _CPUBSUB_H_

#include <stdint.h>

#include "pipe.h"
#include "topic.h"

#define CPS_NETWORK_MID         (0)
#define CPS_PIPE_LENGTH_DEFAULT (10)
#define CPS_RX_BLOCK_INT_MS     (10)

typedef enum
{
    CPS_OK,
    CPS_NO_MSG,
    CPS_ALLOC_ERROR
} cps_result_t;

typedef enum
{
    CPS_SRC_NORMAL,
    CPS_SRC_NETWORK,
} cps_pub_src_t;

typedef struct cps_node_t
{
    struct cps_node_t *next;
    topic_t            topic;
    pipe_t            *pipe;
} cps_node_t;

typedef struct
{
    uint32_t mid;
    void    *ptr;
} cps_packet_template_t;

cps_result_t cps_subscribe(topic_t topic, uint32_t topic_size, pipe_t *pipe);
cps_result_t cps_publish(void *msg, uint32_t mid);
cps_result_t cps_publish_ex(void *msg, uint32_t mid, cps_pub_src_t src);
cps_result_t cps_receive(pipe_t *pipe, void *msg, pipe_wait_t wait);
topic_t      cps_get_mid(void *data);
void         cps_delay_ms(const uint32_t time_ms);

#endif