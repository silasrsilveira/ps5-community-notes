#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

bool record_is_valid(const uint8_t *data, size_t size);

int main(void) {
    const uint8_t valid_record[] = {
        'L', 'A', 'B', '1',
        0x02, 0x00, 0x00, 0x00,
        0xAA, 0xBB
    };

    const uint8_t truncated_record[] = {
        'L', 'A', 'B', '1',
        0x03, 0x00, 0x00, 0x00,
        0xAA, 0xBB
    };

    assert(record_is_valid(valid_record, sizeof valid_record));
    assert(!record_is_valid(truncated_record, sizeof truncated_record));

    puts("Record tests passed");
    return 0;
}