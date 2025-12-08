#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day07.inc"
};
#else
char in[] = {
".......S.......\n"
"...............\n"
".......^.......\n"
"...............\n"
"......^.^......\n"
"...............\n"
".....^.^.^.....\n"
"...............\n"
"....^.^...^....\n"
"...............\n"
"...^.^...^.^...\n"
"...............\n"
"..^...^.....^..\n"
"...............\n"
".^.^.^.^.^...^.\n"
"...............\n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day07.ispc";

static const char *step_fn_name = "step";
using step_fn_t = void (*)(u64 beams[], u64 num_beams, u64 *num_splits);

struct coord {
    union {
        struct { i32 x; i32 y; };
        u64 key;
    };
};

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "" }, ispc_src);
    step_fn_t step_fn = (step_fn_t)engine->GetJitFunction(step_fn_name);

    printf("%s", in);
    u64 num_splits = 0;
    step_fn(nullptr, 69, &num_splits);
    printf(" -> num_splits = %lld\n", num_splits);

    coord test;
    test.key = num_splits;
    printf("Got coord from ISPC : (%i, %i)\n", test.x, test.y);

    print_res(p1, "%lld", 0LL);
    print_res(p2, "%lld", 0LL);
    return 0;
}
