#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define TEST_RANGE 100000
#define MSG_LEN 8

static inline bool is_divisible(uint32_t n, uint64_t M) {
    return n * M <= M - 1;
}

static uint64_t M3 = UINT64_C(0xFFFFFFFFFFFFFFFF) / 3 + 1;
static uint64_t M5 = UINT64_C(0xFFFFFFFFFFFFFFFF) / 5 + 1;

int main(int argc, char *argv[]) {
    long long time1 = 0LL, time2 = 0LL;
    struct timespec tt1, tt2;
    clock_gettime(CLOCK_MONOTONIC, &tt1);
    for (unsigned int i = 1; i <= TEST_RANGE; i++) {
        char s[MSG_LEN + 1];
        int div3 = i % 3;
        int div5 = i % 5;
        if (!div3) sprintf(s, "Fizz");
        if (!div5) sprintf(s, "Buzz");
        if (!div3 && !div5) sprintf(s, "FizzBuzz");
        if (div3 && div5) sprintf(s, "%u", i);
        sprintf(s, "%s\n", s);
        // printf("%s", s);
    }
    clock_gettime(CLOCK_MONOTONIC, &tt2);
    time1 += (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

    clock_gettime(CLOCK_MONOTONIC, &tt1);
    for (unsigned int i = 1; i <= TEST_RANGE; i++) {
        uint8_t div3 = is_divisible(i, M3);
        uint8_t div5 = is_divisible(i, M5);
        unsigned int length = (2 << div3) << div5;

        char fmt[MSG_LEN + 1];
        strncpy(fmt, &"FizzBuzz%u"[(MSG_LEN >> div5) >> (div3 << 2)], length);
        fmt[length] = '\0';

        sprintf(fmt, "%s\n", fmt);
        // printf(fmt, i);
    }
    clock_gettime(CLOCK_MONOTONIC, &tt2);
    time2 += (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

    printf("%lld %lld\n", time1, time2);
    return 0;
}