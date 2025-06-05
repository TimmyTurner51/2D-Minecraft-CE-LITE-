#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdint.h>

/**
 * Compress a byte array using simple run-length encoding (RLE).
 * 
 * @param dst   Destination buffer (must be large enough)
 * @param src   Source buffer to compress
 * @param len   Length of source buffer
 * @return      Number of bytes written to dst (compressed size)
 */
int rle_compress(uint8_t *dst, const uint8_t *src, int len);

/**
 * Decompress an RLE-compressed byte array.
 * 
 * @param dst   Destination buffer to write decompressed data
 * @param src   Source compressed data
 * @param len   Length of compressed data
 * @return      Number of bytes written to dst (decompressed size)
 */
int rle_decompress(uint8_t *dst, const uint8_t *src, int len);

#endif // COMPRESSOR_H