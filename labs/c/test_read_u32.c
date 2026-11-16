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

int main(void) {
    const uint8_t bytes[] = {0x78, 0x56, 0x34, 0x12};
    uint32_t value = 0;

    assert(read_u32_le(bytes, sizeof bytes, 0, &value));
    assert(value == 0x12345678u);

    assert(!read_u32_le(bytes, 3, 0, &value));
    assert(!read_u32_le(bytes, sizeof bytes, 1, &value));

    puts("Read test passed");
    return 0;
}
