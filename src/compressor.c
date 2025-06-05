#include <stdint.h>
#include <string.h>

// Compress src[len] into dst[], return compressed size
int rle_compress(uint8_t *dst, const uint8_t *src, int len) {
    int out = 0;
    for (int i = 0; i < len;) {
        int run_len = 1;
        while (i + run_len < len && src[i] == src[i + run_len] && run_len < 255) {
            run_len++;
        }

        if (run_len >= 4) {
            dst[out++] = 0xFF;       // Run marker
            dst[out++] = src[i];     // Value
            dst[out++] = run_len;    // Count
            i += run_len;
        } else {
            dst[out++] = 0x00;       // Literal marker
            dst[out++] = src[i++];
        }
    }
    return out;
}

// Decompress src[len] into dst[], return decompressed size
int rle_decompress(uint8_t *dst, const uint8_t *src, int len) {
    int in = 0, out = 0;
    while (in < len) {
        uint8_t flag = src[in++];
        if (flag == 0xFF) {
            uint8_t val = src[in++];
            uint8_t count = src[in++];
            memset(dst + out, val, count);
            out += count;
        } else {
            dst[out++] = src[in++];
        }
    }
    return out;
}