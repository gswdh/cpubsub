#include "sublist.h"

#include <stddef.h>

static sublist_subs_t subs[CPS_MAX_SUBS] = {0};

sublist_result_t sublist_add_sub(const pipe_t *pipe, const uint32_t msg_id)
{
    // Check the pipe is not null
    if (pipe == NULL)
    {
        return SUBLIST_INVALID_PARAM;
    }

    for (uint32_t i = 0U; i < CPS_MAX_SUBS; i++)
    {
        if (subs[i].status == SUBLIST_NO_SUB)
        {
            subs[i].status = SUBLIST_SUBBED;
            subs[i].pipe   = pipe;
            subs[i].msg_id = msg_id;

            return SUBLIST_OK;
        }
    }

    return SUBLIST_NO_SPACE;
}

void sublist_remove_sub(const pipe_t *pipe, const uint32_t msg_id)
{
    // Check the pipe is not null
    if (pipe == NULL)
    {
        return;
    }

    for (uint32_t i = 0U; i < CPS_MAX_SUBS; i++)
    {
        // If the subscription matches...
        if ((subs[i].status == SUBLIST_SUBBED) && (subs[i].pipe == pipe) &&
            (subs[i].msg_id == msg_id))
        {
            // Unsub it
            subs[i].status = SUBLIST_NO_SUB;
        }
    }
}

uint32_t sublist_get_subs(pipe_t **pipes, const uint32_t msg_id, const uint32_t max_pipes)
{
    // Check the pipes array is not null
    if (pipes == NULL)
    {
        return 0U;
    }

    // Check the max pipes is not zero
    if (max_pipes == 0U)
    {
        return 0U;
    }

    uint32_t pipe_count = 0U;
    for (uint32_t i = 0U; i < CPS_MAX_SUBS; i++)
    {
        // If the status is good and the msg_id matches...
        if ((subs[i].status == SUBLIST_SUBBED) && (subs[i].msg_id == msg_id))
        {
            // Copy over the pointer
            pipes[pipe_count++] = subs[i].pipe;
        }

        // Look out for the limit
        if (pipe_count == max_pipes)
        {
            return pipe_count;
        }
    }

    return pipe_count;
}
