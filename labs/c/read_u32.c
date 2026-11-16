#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool read_u32_le(
    const uint8_t *data,
    size_t length,
    size_t offset,
    uint32_t *result
) {
    if (data == NULL || result == NULL) {
        return false;
    }

    if (offset > length || length - offset < sizeof(*result)) {
        return false;
    }

    *result =
        (uint32_t)data[offset]
        | ((uint32_t)data[offset + 1] << 8)
        | ((uint32_t)data[offset + 2] << 16)
        | ((uint32_t)data[offset + 3] << 24);

    return true;
}
