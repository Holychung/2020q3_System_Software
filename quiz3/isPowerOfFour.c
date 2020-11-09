bool isPowerOfFour_new(int num)
{   
    return !(__builtin_popcount(num) ^ 1) && __builtin_ffs(num) & 1;
}