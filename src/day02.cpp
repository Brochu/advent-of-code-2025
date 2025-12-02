#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
//#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day02.inc"
};
#else
char in[] = {
    "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day02.ispc";

//static const char *ispc_fn = "sums";
//using sums_func_t = void (*)(int vals[], int starts[], int lens[], int outs[], int count);

bool validate_num(i64 num) {
    u64 size = 0;
    u64 lo = 1;
    while (lo < num) {
        lo *= 10;
        size++;
    }
    //printf("[CHECK] num = %lld ; size = %lld\n", num, size);

    if ((size % 2) != 0) {
        return false; // Odd sized nums cannot be invalid
    }

    lo = 0;
    for (int i = 0; i < size/2; i++) {
        i64 rem = num % 10;
        rem *= pow(10, i);
        lo += rem;
        num /= 10;
    }
    //printf("[CHECK] hi = %lld ; lo = %lld\n", num, lo);
    return num == lo;
}

bool validate_window(i64 num) {
    bool seen[10];
    memset(seen, 0, sizeof(seen));

    u64 size = 0;
    u64 lo = 1;
    while (lo < num) {
        lo *= 10;
        size++;
    }

    return true;
}

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    //auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);

    u64 sum_p1 = 0;
    u64 sum_p2 = 0;
    for (strview &elem : sv_split(input, ",")) {
        strview min_str, max_str;
        sv_split_once(elem, "-", &min_str, &max_str);

        i64 min, max;
        min = atoll(min_str.ptr);
        max = atoll(max_str.ptr);

        for (i64 num = min; num <= max; num++) {
            if (validate_num(num)) {
                //printf("[VALID] num = %lld\n", num);
                sum_p1 += num;
            }
            if (validate_window(num)) {
                printf("[VALID] num = %lld\n", num);
                sum_p2 += num;
            }
            break;
        }
    }

    print_res(p1, "%lld", sum_p1);
    print_res(p2, "%lld", sum_p2);
    return 0;
}
