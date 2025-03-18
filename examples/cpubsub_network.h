#ifndef _CPUBSUB_NET_H_
#define _CPUBSUB_NET_H_

#include <stdint.h>

#define CMD_MSG_BUFFER_LEN (256)

/*
    Needs implementing by the user. Used to transfer bytes onto the network medium.
*/
void cps_network_transmit(uint8_t *data, uint32_t len);

/*
    Called by the used application when bytes have been recieved on the network medium.
    The data must represent a single whole message.
*/
void cps_network_recieve(char *data, uint32_t len);

void cps_network_task(void *params);

#endif