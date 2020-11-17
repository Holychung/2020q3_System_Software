int totalHammingDistance(int* nums, int numsSize){
    int bit_count[sizeof(int) * 8] = {0};
    for (int i = 0; i < numsSize; i++) {
        int num = nums[i];
        while (num) {
            bit_count[__builtin_ctz(num)]++;
            num ^= num & -num;
        }
    }
    int sum = 0;
    for (int i = 0; i < sizeof(int) * 8; i++) {
        sum += (numsSize - bit_count[i]) * bit_count[i];
    }
    return sum;
}