#include "cpubsub_network.h"

#include "cpubsub.h"
#include "messages.h"

#include <stdio.h>
#include <string.h>

static char *encode_data(const uint8_t *data)
{
    // Encoded data
    switch (messages_msg_mid(data))
    {
    case MSGBackLightPercentage_MID:
        return messages_serialise_msgbacklightpercentage(
            (const MSGBackLightPercentage_t *const)data);
        break;
    case MSGMotorSpeed_MID:
        return messages_serialise_msgmotorspeed((const MSGMotorSpeed_t *const)data);
        break;
    case MSGSensorGain_MID:
        return messages_serialise_msgsensorgain((const MSGSensorGain_t *const)data);
        break;
    case MSGSensorExposure_MID:
        return messages_serialise_msgsensorexposure((const MSGSensorExposure_t *const)data);
        break;
    case MSGScannerStatus_MID:
        return messages_serialise_msgscannerstatus((const MSGScannerStatus_t *const)data);
        break;
    case MSGFPGADone_MID:
        return messages_serialise_msgfpgadone((const MSGFPGADone_t *const)data);
        break;
    default:
        return NULL;
        break;
    }

    return NULL;
}

static uint32_t decode_data(const uint8_t *serialised, uint8_t *deserialised)
{
    // Encoded data
    const uint32_t mid = messages_msg_serialised_mid(serialised);
    switch (mid)
    {
    case MSGBackLightPercentage_MID:
        if (messages_deserialise_msgbacklightpercentage(
                serialised, (MSGBackLightPercentage_t *)deserialised) == false)
        {
            return 0;
        }
        break;
    case MSGMotorSpeed_MID:
        if (messages_deserialise_msgmotorspeed(serialised, (MSGMotorSpeed_t *)deserialised) ==
            false)
        {
            return 0;
        }
        break;
    case MSGSensorGain_MID:
        if (messages_deserialise_msgsensorgain(serialised, (MSGSensorGain_t *)deserialised) ==
            false)
        {
            return 0;
        }
        break;
    case MSGSensorExposure_MID:
        if (messages_deserialise_msgsensorexposure(serialised,
                                                   (MSGSensorExposure_t *)deserialised) == false)
        {
            return 0;
        }
        break;
    case MSGScannerStatus_MID:
        if (messages_deserialise_msgscannerstatus(serialised, (MSGScannerStatus_t *)deserialised) ==
            false)
        {
            return 0;
        }
        break;
    case MSGFPGADone_MID:
        if (messages_deserialise_msgfpgadone(serialised, (MSGFPGADone_t *)deserialised) == false)
        {
            return 0;
        }
        break;
    default:
        return 0;
        break;
    }

    return mid;
}

void cps_network_task(void *params)
{
    pipe_t  pipe                    = {0};
    uint8_t msg[CMD_MSG_BUFFER_LEN] = {0};

    // The CPS_NETWORK_MID acts as a wild card for all messages
    cps_subscribe(CPS_NETWORK_MID, CMD_MSG_BUFFER_LEN, &pipe);

    while (1)
    {
        // Get a message
        cps_receive(&pipe, (void *)msg, PIPE_WAIT_BLOCK);

        // Only if the message is valid
        if (messages_msg_len(cps_get_mid((void *)msg)) != 0)
        {
            // Encode the data
            char *data = encode_data((const uint8_t *)msg);

            // Send on the network
            cps_network_transmit((uint8_t *)data, strlen(data));
            free(data);
        }
    }
}

void __attribute__((weak)) cps_network_transmit(uint8_t *data, uint32_t len) {}

void cps_network_recieve(char *data, uint32_t len)
{
    // Decode the base64
    uint8_t        msg[CMD_MSG_BUFFER_LEN] = {0};
    const uint32_t mid                     = decode_data((const uint8_t *)data, msg);
    if (mid != 0)
    {
        // Send the data in the decoded message
        cps_publish_ex((void *)msg, mid, CPS_SRC_NETWORK);
    }
}
