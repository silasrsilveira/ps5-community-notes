#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool read_u32_le(
    const uint8_t *data,
    size_t length,
    size_t offset,
    uint32_t *result
);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    size_t claimed_length = size;
    size_t offset = 0;

    if (size >= 1) {
        offset = data[0];
        if (offset > size) {
            offset = size;
        }
    }

    if (size >= 2) {
        claimed_length = data[1];
        if (claimed_length > size) {
            claimed_length = size;
        }
    }

    uint32_t value = 0;
    bool expected = false;

    if (offset <= claimed_length) {
        expected = claimed_length - offset >= sizeof value;
    }

    bool actual = read_u32_le(data, claimed_length, offset, &value);
    assert(actual == expected);

    if (actual) {
        uint32_t expected_value =
            (uint32_t)data[offset]
            | ((uint32_t)data[offset + 1] << 8)
            | ((uint32_t)data[offset + 2] << 16)
            | ((uint32_t)data[offset + 3] << 24);

        assert(value == expected_value);
    }

    return 0;
}