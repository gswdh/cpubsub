#ifndef _BS64_ENC_H_
#define _BS64_ENC_H_

#include <stdint.h>

uint32_t encode_data(const uint8_t *data, const uint32_t len,
                     const char *encoded_data,
                     const uint32_t encoded_data_max_len);

uint32_t decode_data(const char *encoded_data, const uint32_t encoded_len,
                     const uint8_t *output_data,
                     const uint32_t output_data_max_len);

#endif