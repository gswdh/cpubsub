#ifndef _CPUBSUB_H_
#define _CPUBSUB_H_

#include <stdint.h>

#include "pipe.h"

#if defined(CPS_HEADER_USE_TIME_H) && !defined(CPS_HEADER_USE_TIMESTAMP)
#error "CPS_HEADER_USE_TIMESTAMP must be defined to use CPS_HEADER_USE_TIME_H"
#endif

typedef enum
{
    CPS_OK = 0U,
    CPS_INVALID_PARAM,
    CPS_NO_MSG,
    CPS_SUB_ERROR,
    CPS_PUB_ERROR,
    CPS_PIPE_INIT_ERROR,
} cps_result_t;

typedef enum
{
    CPS_WAIT_BLOCK = 0U,
    CPS_WAIT_POLL,
} cps_wait_t;

#ifdef CPS_HEADER_USE_TIMESTAMP
typedef struct
{
    uint32_t seconds_s;
    uint32_t subseconds_us;
} cps_packet_time_t;
#endif

typedef struct
{
    uint32_t msg_id;
    uint32_t sender_id;
#ifdef CPS_HEADER_USE_TIMESTAMP
    cps_packet_time_t time;
#endif
} cps_packet_header_t;

typedef struct
{
    cps_packet_header_t header;
} cps_packet_template_t;

/**
 * @brief Subscribes a pipe to receive messages of a specific ID.
 *
 * This function ensures that the pipe has a valid queue length and initializes it accordingly.
 * It then registers the pipe for messages with the given `msg_id` and sets its type based on
 * the source (`src`).
 *
 * @param pipe Pointer to the pipe structure.
 * @param msg_id The message ID to subscribe to.
 * @param msg_size The size of each message.
 * @param src The source of the published messages (normal or network).
 * @return CPS_OK if subscription is successful.
 * @return CPS_PIPE_INIT_ERROR if the pipe initialization fails.
 * @return CPS_SUB_ERROR if adding the subscription fails.
 * @return CPS_INVALID_PARAM if an invalid source type is provided.
 */
cps_result_t cps_subscribe(pipe_t *pipe, const uint32_t msg_id, const uint32_t msg_size);

/**
 * @brief Initialises the message header with a given message ID.
 *
 * This function initializes the header of the message with the provided message ID.
 *
 * @param header Pointer to the message header to initialize.
 * @param msg_id The message ID to set in the header.
 */
void cps_init_msg_header(void *msg, const uint32_t device_id, const uint32_t msg_id);

/**
 * @brief Publishes a message to all subscribed pipes with a normal publication source.
 *
 * This is a wrapper function for `cps_publish_ex` that uses a default publication source of
 * `CPS_SRC_NORMAL`.
 *
 * @param msg Pointer to the message to publish.
 * @return CPS_OK if the message was successfully published, an error code otherwise.
 */
cps_result_t cps_publish(const void *msg);

/**
 * @brief Publishes a message to all subscribed pipes with a normal publication source.
 *
 * This is a wrapper function for `cps_publish_ex` that uses a default publication source of
 * `CPS_SRC_NORMAL`.
 *
 * @param msg Pointer to the message to publish.
 * @return CPS_OK if the message was successfully published, an error code otherwise.
 */
#ifdef CPS_HEADER_USE_NETWORKING
cps_result_t cps_publish_from_network(const void *msg, const uint32_t device_id);
#endif

/**
 * @brief Receives a message from a specified pipe.
 *
 * This function waits for or polls a message from the specified pipe based on the provided wait
 * mode.
 *
 * @param pipe Pointer to the pipe from which to receive the message.
 * @param msg Pointer to the message buffer to store the received message.
 * @param wait The wait mode: either `PIPE_WAIT_BLOCK` or `PIPE_WAIT_POLL`.
 * @return CPS_OK if a message was received, CPS_NO_MSG if no message is available.
 */
cps_result_t cps_receive(const pipe_t *pipe, void *msg, const cps_wait_t wait);

/**
 * @brief Delays the program for a specified number of milliseconds.
 *
 * This function provides a delay for the specified duration in milliseconds.
 * It should be implemented by the user to suit the platform-specific timing requirements.
 *
 * @param time_ms The number of milliseconds to delay the execution.
 */
void cps_delay_ms(const uint32_t time_ms);

/**
 * @brief Gets the system time.
 *
 * This function provides access to the system time to the cpubsub library.
 * It should be implemented by the user to suit the platform-specific time requirements, the
 * implementation is up to the user.
 *
 * @return cps_packet_time_t filled by the user's code.
 */
#if defined(CPS_HEADER_USE_TIMESTAMP) && !defined(CPS_HEADER_USE_TIME_H)
cps_packet_time_t cps_get_time(void);
#endif

#endif