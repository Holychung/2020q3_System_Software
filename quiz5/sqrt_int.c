int mySqrt(int x){
    if (x == 0 || x == 1)
        return x;
    if (x == INT_MAX) // avoid overflow
        x-=1;
    
    unsigned int a; // using right shift
    a = (x + 1) >> 1;
    
    while (a > x/a){
        a = (a + x/a) >> 1;
    }
    return a;
}

int mySqrt_fast(int v)
{
    unsigned long temp, nHat = 0, b = 0x8000, bshft = 15;
    do{
        if (v >= (temp = (((nHat << 1) + b) << bshft--)))
        {
            nHat += b;
            v -= temp;
        }
    } while (b >>= 1);
    return nHat;
}