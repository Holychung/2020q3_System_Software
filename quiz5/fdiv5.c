#include <stdio.h>
#include <stdlib.h>

double divop(double orig, int slots) {
    if (slots == 1 || orig == 0)
        return orig;
    int od = slots & 1;
    double result = divop(orig / D1, od ? (slots + D2) >> 1 : slots >> 1);
    if (od)
        result += divop(result, slots);
    return result;
}