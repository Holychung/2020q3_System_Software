#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define RINGBUF_DECL(T, NAME) \
    typedef struct {          \
        int size;             \
        int start, end;       \
        T *elements;          \
    } NAME

RINGBUF_DECL(int, int_buf);

#define RINGBUF_INIT(BUF, S, T)             \
    {                                       \
        static T static_ringbuf_mem[S + 1]; \
        BUF.elements = static_ringbuf_mem;  \
    }                                       \
    BUF.size = S;                           \
    BUF.start = 0;                          \
    BUF.end = 0;

#define NEXT_START_INDEX(BUF) \
    (((BUF)->start != (BUF)->size) ? ((BUF)->start + 1) : 0)
#define NEXT_END_INDEX(BUF) (((BUF)->end != (BUF)->size) ? ((BUF)->end + 1) : 0)

#define is_ringbuf_empty(BUF) ((BUF)->end == (BUF)->start)
#define is_ringbuf_full(BUF) (NEXT_END_INDEX(BUF) == (BUF)->start)

#define ringbuf_write_peek(BUF) (BUF)->elements[(BUF)->end]
#define ringbuf_write_skip(BUF)                   \
    do {                                          \
        (BUF)->end = NEXT_END_INDEX(BUF);         \
        if (is_ringbuf_empty(BUF))                \
            (BUF)->start = NEXT_START_INDEX(BUF); \
    } while (0)

#define ringbuf_write_only(BUF) (BUF)->end = NEXT_END_INDEX(BUF);

#define ringbuf_read_peek(BUF) (BUF)->elements[(BUF)->start]
#define ringbuf_read_skip(BUF) (BUF)->start = NEXT_START_INDEX(BUF);

#define ringbuf_write(BUF, ELEMENT)        \
    do {                                   \
        ringbuf_write_peek(BUF) = ELEMENT; \
        ringbuf_write_skip(BUF);           \
    } while (0)

#define ringbuf_read(BUF, ELEMENT)        \
    do {                                  \
        ELEMENT = ringbuf_read_peek(BUF); \
        ringbuf_read_skip(BUF);           \
    } while (0)

#define ringbuf_write_array_for(BUF, ELEMENTS, SIZE)                \
    do {                                                           \
        for (int i = 0; i < SIZE; i++) { \
            ringbuf_write_peek(BUF) = ELEMENTS[i]; \
            ringbuf_write_skip(BUF); \
        } \
    } while(0)

#define ringbuf_write_array(BUF, ELEMENTS, SIZE, T)                \
    do {                                                           \
        size_t part1, part1_sz, part2_sz;                          \
        part1 = min(SIZE, (BUF).size + 1 - (BUF).end);             \
        part1_sz = part1 * sizeof(T);                              \
        part2_sz = SIZE * sizeof(T) - part1_sz;                    \
        memcpy((BUF).elements + (BUF).end, ELEMENTS, part1_sz);    \
        memcpy((BUF).elements, ELEMENTS + part1, part2_sz);        \
        (BUF).end = ((BUF).end + SIZE) % ((BUF).size + 1);         \
    } while(0)

#define V_RINGBUF_INIT(BUF, S, T)                                             \
    do {                                                                      \
        const size_t pagesize = getpagesize();                                \
        size_t size = ((float)(S * sizeof(T)) / pagesize) * (pagesize + 1);   \
        const int fd = fileno(tmpfile());                                     \
        if (fd == -1) {                                                       \
            fprintf(stderr, "error on open temporary file");                  \
            exit(1);                                                          \
        }                                                                     \
        ftruncate(fd, size);                                                  \
        BUF.elements = mmap(NULL, 2 * size, PROT_NONE,                        \
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);                   \
        if (BUF.elements == MAP_FAILED) {                                     \
            fprintf(stderr, "error on mapping");                              \
            exit(1);                                                          \
        }                                                                     \
        mmap(BUF.elements, size, PROT_READ | PROT_WRITE,                      \
                       MAP_SHARED | MAP_FIXED, fd, 0);                        \
        mmap(BUF.elements + size / sizeof(T), size,                           \
                       PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd, 0);\
        BUF.size = size / sizeof(T);                                          \
        BUF.start = 0;                                                        \
        BUF.end = 0;                                                          \
    } while(0)                                                                \

#define v_ringbuf_write_array(BUF, ELEMENTS, SIZE, T)                        \
    do {                                                                   \
        memcpy((BUF).elements + (BUF).end, ELEMENTS, SIZE * sizeof(T));  \
        (BUF).end = ((BUF).end + SIZE) % ((BUF).size + 1);              \
    } while(0)

int main()
{
    // int_buf my_buf;
    // RINGBUF_INIT(my_buf, 2, int);
    // assert(is_ringbuf_empty(&my_buf));

    // ringbuf_write(&my_buf, 37);
    // ringbuf_write(&my_buf, 72);
    // assert(!is_ringbuf_empty(&my_buf));

    // int first;
    // ringbuf_read(&my_buf, first);
    // assert(first == 37);

    // int second;
    // ringbuf_read(&my_buf, second);
    // assert(second == 72);
    
    int_buf my_buf_1, my_buf_2, my_buf_3;
    RINGBUF_INIT(my_buf_1, 1024, int);
    RINGBUF_INIT(my_buf_2, 1024, int);
    V_RINGBUF_INIT(my_buf_3, 1024, int);

    int tmp[1000];
    for (int i = 0; i < 1000; i++)
        tmp[i] = i;
    

    for (int i = 0; i < 100; i++) {
        struct timespec tt1, tt2;

        // using forloop
        clock_gettime(CLOCK_MONOTONIC, &tt1);
        ringbuf_write_array_for(&my_buf_1, tmp, 1000);
        clock_gettime(CLOCK_MONOTONIC, &tt2);
        long long time1 = (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                          (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);
        // using memcpy 
        clock_gettime(CLOCK_MONOTONIC, &tt1);
        ringbuf_write_array(my_buf_2, tmp, 1000, int);
        clock_gettime(CLOCK_MONOTONIC, &tt2);
        long long time2 = (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                          (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

        // using virtual memory mapping trick
        clock_gettime(CLOCK_MONOTONIC, &tt1);
        v_ringbuf_write_array(my_buf_3, tmp, 1000, int);
        clock_gettime(CLOCK_MONOTONIC, &tt2);
        long long time3 = (long long) (tt2.tv_sec * 1e9 + tt2.tv_nsec) -
                          (long long) (tt1.tv_sec * 1e9 + tt1.tv_nsec);

        printf("%d %lld %lld %lld\n", i, time1, time2, time3);
    }

    return 0;
}