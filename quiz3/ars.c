#include <stdio.h>

int asr_i(signed int m, unsigned int n)
{
    const int logical = (((int) -1) >> 1) > 0;
    unsigned int fixu = -(logical & (m < 0));
    int fix = *(int *) &fixu;
    return (m >> n) | (fix ^ (fix >> n));
}

int main(int argc, char const *argv[])
{	
	printf("%d\n", asr_i(5, 1));
	// 5 / 2 = 2
	printf("%d\n", asr_i(-5, 1));
	// -5 / 2 = -3
	return 0;
}