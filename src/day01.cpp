#include <cstdlib>
#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

char in[] = {
    #include "../inputs/day01.inc"
};
strview input { in, strlen(in) };

char t[] = {
"L68\n"
"L30\n"
"R48\n"
"L5\n"
"R60\n"
"L55\n"
"L1\n"
"L99\n"
"R14\n"
"L82"
};
strview test { t, strlen(t) };

static const char *ispc_src = "./shaders/day01.ispc";

//static const char *ispc_fn = "sums";
//using sums_func_t = void (*)(int vals[], int starts[], int lens[], int outs[], int count);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);
    //std::vector<strview> lines = sv_split(input, "\n");
    std::vector<strview> lines = sv_split(test, "\n");

    int num_zero_p1 = 0;
    int num_zero_p2 = 0;
    int dial_pos = 50;
    for (strview &v : lines) {
        char op = v.ptr[0];
        int len = atoi(&v.ptr[1]);
        //printf(" -> Move %i steps to the %c\n", len, op);

        num_zero_p2 += len/100;
        if (op == 'L') {
            dial_pos = (dial_pos - len);
        } else if (op == 'R') {
            dial_pos = (dial_pos + len);
        }
        dial_pos %= 100;

        //printf(" -> dial_pos after move = %i\n", dial_pos);
        if (dial_pos % 100 == 0) {
            num_zero_p1++;
        }
    }


    print_res(p1, "%i", num_zero_p1);
    print_res(p2, "%i", num_zero_p2);
    return 0;
}
