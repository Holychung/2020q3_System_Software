#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })

uint64_t gcd64(uint64_t u, uint64_t v) {
    if (!u || !v) return u | v;
    int shift;
    for (shift = 0; !((u | v) & 1); shift++) {
        u /= 2, v /= 2;
    }
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

uint64_t faster_gcd64(uint64_t u, uint64_t v) {
    if (!u || !v) return u | v;
    
    int shift = min(__builtin_ctz(u), __builtin_ctz(v));
    u >>= shift;
    v >>= shift;

    while (!(u & 1))
        u >>= __builtin_ctz(u);
    do {
        while (!(v & 1))
            v >>= __builtin_ctz(v);
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

int main(int argc, char const *argv[]) {
    for (int i = 1; i < 50000; i++) {
        long long time1 = 0LL, time2 = 0LL;
        int gcd, faster_gcd;
        for (int j = 1; j < i; j++) {
            struct timespec tt1, tt2;           
            clock_gettime(CLOCK_MONOTONIC, &tt1);
            gcd = gcd64(i, j);
            clock_gettime(CLOCK_MONOTONIC, &tt2);
            time1 += (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                        (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC, &tt1);
            faster_gcd = faster_gcd64(i, j);
            clock_gettime(CLOCK_MONOTONIC, &tt2);
            time2 += (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                        (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);
        }
        printf("%d %lld %lld\n", i, time1, time2);
    }

    return 0;
}