#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

bool read_u32_le(
    const uint8_t *data,
    size_t length,
    size_t offset,
    uint32_t *result
);

static uint32_t next_random(uint32_t *state) {
    *state = *state * UINT32_C(1664525) + UINT32_C(1013904223);
    return *state;
}

int main(void) {
    uint32_t state = UINT32_C(0xC0FFEE);
    uint8_t bytes[16];

    for (size_t trial = 0; trial < 100000; ++trial) {
        for (size_t i = 0; i < sizeof bytes; ++i) {
            bytes[i] = (uint8_t)(next_random(&state) >> 24);
        }

        size_t length = next_random(&state) % (sizeof bytes + 1);
        size_t offset = next_random(&state) % (sizeof bytes + 2);
        uint32_t value = 0;

        bool expected = false;
        if (offset <= length) {
            expected = length - offset >= sizeof value;
        }

        bool actual = read_u32_le(bytes, length, offset, &value);
        assert(actual == expected);

        if (expected) {
            uint32_t expected_value =
                (uint32_t)bytes[offset]
                | ((uint32_t)bytes[offset + 1] << 8)
                | ((uint32_t)bytes[offset + 2] << 16)
                | ((uint32_t)bytes[offset + 3] << 24);

            assert(value == expected_value);
        }
    }

    puts("100000 deterministic cases passed");
    return 0;
}