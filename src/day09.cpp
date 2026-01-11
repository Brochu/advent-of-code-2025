#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
char in[] = {
    #include "../inputs/day09.inc"
};
#else
char in[] = {
"7,1\n"
"11,1\n"
"11,7\n"
"9,7\n"
"9,5\n"
"2,5\n"
"2,3\n"
"7,3\n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day09.ispc";

static const char *solve_fn_name = "solve";
using solve_fn_t = i64 (*)(i64 xs[], i64 ys[], i32 len);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    solve_fn_t solve_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    std::vector<i64> xs;
    std::vector<i64> ys;
    i32 n = 0;
    for (strview line : sv_split(input, "\n")) {
        strview x, y;
        sv_split_once(line, ",", &x, &y);
        xs.emplace_back(atoi(x.ptr));
        ys.emplace_back(atoi(y.ptr));
        n++;
    }

    i64 p1_res = solve_fn(xs.data(), ys.data(), n);

    print_res(p1, "%lli", p1_res);
    print_res(p2, "%i", 0);
    return 0;
}
