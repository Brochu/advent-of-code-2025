#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
//#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day04.inc"
};
#else
char in[] = {
"..@@.@@@@.\n"
"@@@.@.@.@@\n"
"@@@@@.@.@@\n"
"@.@@@@..@.\n"
"@@.@@@@.@@\n"
".@@@@@@@.@\n"
".@.@.@.@@@\n"
"@.@@@.@@@@\n"
".@@@@@@@@.\n"
"@.@.@@@.@.\n"
};
#endif
strview input { in, strlen(in) };

//static const char *ispc_src = "./shaders/day03.ispc";

//static const char *ispc_fn = "solve";
//using solve_func_t = i64 (*)(i8 batteries[], u64 banks[], u64 num_banks, u64 bank_len, i8 k);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
//    auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);
//    solve_func_t solve_fn = (solve_func_t)engine->GetJitFunction(ispc_fn);

    printf("%s\n", in);

    print_res(p1, "%lld", 0LL);
    print_res(p2, "%lld", 0LL);
    return 0;
}
