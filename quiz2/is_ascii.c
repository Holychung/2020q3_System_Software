#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

bool is_ascii(const char str[], size_t size)
{
    if (size == 0)
        return false;
    int i = 0;
    while ((i + 8) <= size) {
        uint64_t payload;
        memcpy(&payload, str + i, 8);
        if (payload & 0x8080808080808080)
            return false;
        i += 8;
    }
    while (i < size) {
        if (str[i] & 0x80)
            return false;
        i++;
    }
    return true;
}

int main(){
	char s1[] = "abcdefg1234 !\n\t";
	char s2[] = {1, 2 , 0xFF, 'a'};
	printf("%d\n", is_ascii(s1, strlen(s1)));
	printf("%d\n", is_ascii(s2, strlen(s2)));
	return 0;
}