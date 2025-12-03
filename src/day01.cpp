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
    //auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);
    std::vector<strview> lines = sv_split(input, "\n");
    //std::vector<strview> lines = sv_split(test, "\n");

    int pos = 50;
    int part1 = 0;
    int part2 = 0;

    for (strview &v : lines) {
        char op = v.ptr[0];
        int len = atoi(&v.ptr[1]);

        if (op == 'L') {
            if (pos != 0 && pos < (len%100)) {
                //printf("\tShould roll over 0 from L\n");
                part2++;
            }
            pos = (pos - len % 100 + 100) % 100;
        } else {
            if (pos != 0 && (100-pos) < (len%100)) {
                //printf("\tShould roll over 0 from R\n");
                part2++;
            }
            pos = (pos + len) % 100;
        }
        //printf("[%c][%i] new pos = %i\n", op, len, pos);

        part2 += len/100;
        if (pos == 0) {
            part1++;
            part2++;
        }
    }

    print_res(p1, "%i", part1);
    print_res(p2, "%i", part2);
    return 0;
}
