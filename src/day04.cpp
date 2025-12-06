#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
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

static const char *ispc_src = "./shaders/day04.ispc";

static const char *ispc_fn = "solve";
using solve_func_t = i64 (*)(i8 cells[], i64 w, i64 h, i8 roll_val, i8 removed[]);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);
    solve_func_t solve_fn = (solve_func_t)engine->GetJitFunction(ispc_fn);
    std::vector<i8> cells;
    i64 width = 0;
    i64 height = 0;

    for (strview line : sv_split(input, "\n")) {
        width = line.len;
        for (i32 i = 0; i < line.len; i++) {
            cells.emplace_back(line.ptr[i]);
        }
        height++;
    }

    std::vector<i8> removed;
    removed.resize(width*height);
    memset(removed.data(), 0, sizeof(i8) * removed.size());
    i64 curr = solve_fn(cells.data(), width, height, '@', removed.data());
    i64 count_p1 = curr;
    i64 count_p2 = count_p1;

    while (curr != 0) {
        for (int i = 0; i < width*height; i++) {
            if (removed[i] > 0) {
                cells[i] = '.';
            }
        }
        memset(removed.data(), 0, sizeof(i8) * removed.size());
        curr = solve_fn(cells.data(), width, height, '@', removed.data());
        count_p2 += curr;
    }

    print_res(p1, "%lld", count_p1);
    print_res(p2, "%lld", count_p2);
    return 0;
}
