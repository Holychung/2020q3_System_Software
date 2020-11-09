bool isPowerOfFour_new(int num)
{   
    int ffs = __builtin_ffs(num);
    return (ffs & 1) && !(num >> ffs);
}