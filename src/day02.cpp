#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

char in[] = {
    #include "../inputs/day02.inc"
};
strview input { in, strlen(in) };

char t[] = {
    ""
};
strview test { t, strlen(t) };

static const char *ispc_src = "./shaders/day02.ispc";

//static const char *ispc_fn = "sums";
//using sums_func_t = void (*)(int vals[], int starts[], int lens[], int outs[], int count);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);

    print_res(p1, "%i", 0);
    print_res(p2, "%i", 0);
    return 0;
}
