#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

bool read_u32_le(
    const uint8_t *data,
    size_t length,
    size_t offset,
    uint32_t *result
);

bool record_is_valid(const uint8_t *data, size_t size) {
    const size_t header_size = 8;

    if (data == NULL || size < header_size) {
        return false;
    }

    if (memcmp(data, "LAB1", 4) != 0) {
        return false;
    }

    uint32_t payload_length = 0;
    if (!read_u32_le(data, size, 4, &payload_length)) {
        return false;
    }

    return (size_t)payload_length <= size - header_size;
}
