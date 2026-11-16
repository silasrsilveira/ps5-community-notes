#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool record_is_valid(const uint8_t *data, size_t size);

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    bool accepted = record_is_valid(data, size);

    if (accepted) {
        assert(data != NULL);
        assert(size >= 8);

        assert(data[0] == 'L');
        assert(data[1] == 'A');
        assert(data[2] == 'B');
        assert(data[3] == '1');

        uint32_t payload_length =
            (uint32_t)data[4]
            | ((uint32_t)data[5] << 8)
            | ((uint32_t)data[6] << 16)
            | ((uint32_t)data[7] << 24);

        assert((size_t)payload_length <= size - 8);
    }

    return 0;
}
