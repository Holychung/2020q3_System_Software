#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define MASK 0x40

uint32_t hexchar2val(char *in)
{
    size_t len = strlen(in);
    assert(len);
    assert(in[0] == '0' && (in[1] == 'x' || in[1] == 'X'));

    uint32_t val = 0;
    for (int i = 2; i < len; i++) {
        const uint8_t letter = in[i] & MASK;
        const uint8_t shift = (letter >> 3) | (letter >> 6);
        val |= ((in[i] + shift) & 0xf) << 4 * (len - i - 1);
    }
    return val;
}

int main(int argc, char const *argv[])
{   
    printf("%u\n", hexchar2val("0xF"));
    // 15
    printf("%u\n", hexchar2val("0xf"));
    // 15
    printf("%u\n", hexchar2val("0xFF"));
    // 255
    printf("%u\n", hexchar2val("0xCAFEBABE"));
    // 3405691582
    printf("%u\n", hexchar2val("0x8BADF00D"));
    // 2343432205
    return 0;
}