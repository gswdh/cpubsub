#include "cpubsub_network.h"

#include "cpubsub.h"

#include "base64.h"

#include <assert.h>

static uint32_t encode_data(const uint8_t *data,
                            const uint32_t len,
                            char          *encoded_data,
                            const uint32_t encoded_data_max_len)
{
    // Calc the encoded data length
    uint32_t encoded_data_len = BASE64_ENCODE_OUT_SIZE(len);

    // Check if we fit
    if (encoded_data_len > encoded_data_max_len)
    {
        return 0;
    }

    // Encode and add a terminator
    base64_encode(data, len, encoded_data);

    // Return the length for TX
    return encoded_data_len;
}

static uint32_t decode_data(const char    *encoded_data,
                            const uint32_t encoded_len,
                            uint8_t       *output_data,
                            const uint32_t output_data_max_len)
{
    // Calc the encoded data length
    uint32_t decoded_data_len = BASE64_DECODE_OUT_SIZE(encoded_len);

    // Check if we fit
    if (decoded_data_len > output_data_max_len)
    {
        return 0;
    }

    // Encode and add a terminator
    base64_decode(encoded_data, encoded_len, output_data);

    // Return the length for TX
    return decoded_data_len;
}

void cps_network_task(void)
{
    pipe_t   pipe                             = {0};
    uint8_t *msg[CMD_MSG_BUFFER_LEN]          = {0};
    char    *encoded_data[CMD_MSG_BUFFER_LEN] = {0};

    // The CPS_NETWORK_MID acts as a wild card for all messages
    cps_subscribe(CPS_NETWORK_MID, CMD_MSG_BUFFER_LEN, &pipe);

    while (1)
    {
        // Get a message
        cps_receive(&pipe, (void *)msg, PIPE_WAIT_POLL);

        // Get this message type length
        uint32_t msg_len = 10; // TODO:

        // Encode the data
        uint32_t encoded_len = encode_data(msg, msg_len, encoded_data, CMD_MSG_BUFFER_LEN);

        // Send on the network
        cps_network_transmit(encoded_data, encoded_len);
    }
}

void __attribute__((weak)) cps_network_transmit(uint8_t *data, uint32_t len) {}

void cps_network_recieve(char *data, uint32_t len)
{
    // Decode the base64
    uint8_t msg[CMD_MSG_BUFFER_LEN] = {0};
    decode_data(data, len, msg, CMD_MSG_BUFFER_LEN);

    // Only send if the MID is non NULL
    uint32_t mid = cps_get_mid((void *)msg);

    if (mid)
    {
        // Send the data in the decoded message
        cps_publish_ex((void *)msg, CPS_SRC_NETWORK);
    }
}
