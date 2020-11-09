#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

size_t naive(uint64_t *bitmap, size_t bitmapsize, uint32_t *out)
{
    size_t pos = 0;
    for (size_t k = 0; k < bitmapsize; ++k) {
        uint64_t bitset = bitmap[k];
        size_t p = k * 64;
        for (int i = 0; i < 64; i++) {
            if ((bitset >> i) & 0x1)
                out[pos++] = p + i;
        }
    }
    return pos;
}

size_t improved(uint64_t *bitmap, size_t bitmapsize, uint32_t *out)
{
    size_t pos = 0;
    uint64_t bitset;
    for (size_t k = 0; k < bitmapsize; ++k) {
        bitset = bitmap[k];
        while (bitset != 0) {
            uint64_t t = bitset & -bitset;
            int r = __builtin_ctzll(bitset);
            out[pos++] = k * 64 + r;
            bitset ^= t;                           
        }
    }
    return pos;
}

int main(int argc, char const *argv[])
{
	int len = 100;
	uint64_t *bitmap1 = malloc(len * sizeof(uint64_t));
	uint64_t *bitmap2 = malloc(len * sizeof(uint64_t));
	uint32_t *out1 = malloc(len * 64 * sizeof(uint32_t));
	uint32_t *out2 = malloc(len * 64 * sizeof(uint32_t));
	memset(bitmap1, 0, len);
	memset(bitmap2, 0, len);
	memset(out1, 0, len * 64);
	memset(out2, 0, len * 64);
	for (int i = 0; i< len; i++) {
		bitmap1[i] = i;
		bitmap2[i] = i;		
	}
	printf("%ld\n", naive(bitmap1, len, out1));
	printf("%ld\n", improved(bitmap2, len, out2));
	for (int i = 0; i< len; i++) {
		if (out1[i] == out2[i])
			printf("%d: correct\n", i);
		else
			printf("%d: failed\n", i);
	}

	return 0;
}