#include "base64_processor.h"

#include "base64.h"

uint32_t encode_data(const uint8_t *data, const uint32_t len,
                     const char *encoded_data,
                     const uint32_t encoded_data_max_len) {
    // Calc the encoded data length
    uint32_t encoded_data_len = BASE64_ENCODE_OUT_SIZE(len);

    // Check if we fit
    if (encoded_data_len > encoded_data_max_len) {
        return 0;
    }

    // Encode and add a terminator
    base64_encode(data, len, encoded_data);

    // Return the length for TX
    return encoded_data_len;
}

uint32_t decode_data(const char *encoded_data, const uint32_t encoded_len,
                     const uint8_t *output_data,
                     const uint32_t output_data_max_len) {
        // Calc the encoded data length
    uint32_t decoded_data_len = BASE64_DECODE_OUT_SIZE(encoded_len);

    // Check if we fit
    if (decoded_data_len > output_data_max_len) {
        return 0;
    }

    // Encode and add a terminator
    base64_decode(encoded_data, encoded_len, output_data);

    // Return the length for TX
    return decoded_data_len;
}