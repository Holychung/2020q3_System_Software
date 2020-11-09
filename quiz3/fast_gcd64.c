#include <stdio.h>
#include <stdint.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

uint64_t gcd64(uint64_t u, uint64_t v) {
    if (!u || !v) return u | v;
    
    int shift = min(__builtin_ctz(u), __builtin_ctz(v));
    u >>= shift;
    v >>= shift;

    while (!(u & 1))
        u /= 2;
    do {
        while (!(v & 1))
            v /= 2;
        if (u < v) {
            v -= u;
        } else {
            uint64_t t = u - v;
            u = v;
            v = t;
        }
    } while (v);
    return u << shift;
}

int main(int argc, char const *argv[])
{
	printf("gcd: %lu\n", gcd64(12, 8));
	// 4
	printf("gcd: %lu\n", gcd64(252, 105));
	// 21
	
	return 0;
}