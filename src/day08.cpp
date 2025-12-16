#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day08.inc"
};
const static i32 ITER = 1000;
#else
char in[] = {
"162,817,812\n"
"57,618,57\n"
"906,360,560\n"
"592,479,940\n"
"352,342,300\n"
"466,668,158\n"
"542,29,236\n"
"431,825,988\n"
"739,650,466\n"
"52,470,668\n"
"216,146,977\n"
"819,987,18\n"
"117,168,530\n"
"805,96,715\n"
"346,949,466\n"
"970,615,88\n"
"941,993,340\n"
"862,61,35\n"
"984,92,344\n"
"425,690,689\n"
};
const static i32 ITER = 10;
#endif
strview input { in, strlen(in) };

struct boxes {
    std::vector<i32> xs;
    std::vector<i32> ys;
    std::vector<i32> zs;
};

static const char *ispc_src = "./shaders/day08.ispc";

static const char *solve_fn_name = "solvep1";
using solve_fn_t = i32 (*)(i32 x[], i32 y[], i32 z[], i32 num_boxes, i32 num_iter, i32 *one, i32 *two);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    solve_fn_t solvep1_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    boxes b;
    for(strview box_line : sv_split(input, "\n")) {
        strview lo, rest;
        sv_split_once(box_line, ",", &lo, &rest);

        strview mid, hi;
        sv_split_once(rest, ",", &mid, &hi);

        b.xs.emplace_back(atoi(lo.ptr));
        b.ys.emplace_back(atoi(mid.ptr));
        b.zs.emplace_back(atoi(hi.ptr));
    }

    printf("[DEBUG] Found %lld boxes\n", b.xs.size());

    i32 from, to;
    i32 p1_res = solvep1_fn(b.xs.data(), b.ys.data(), b.zs.data(), b.xs.size(), ITER, &from, &to);

    printf("[DEBUG] from %d -> (%i, %i, %i)\n", from, b.xs[from], b.ys[from], b.zs[from]);
    printf("[DEBUG] to   %d -> (%i, %i, %i)\n", to, b.xs[to], b.ys[to], b.zs[to]);

    print_res(p1, "%i", p1_res);
    print_res(p2, "%i", 0);
    return 0;
}
