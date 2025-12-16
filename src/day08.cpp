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

struct box {
    i32 x, y, z;
};

static const char *ispc_src = "./shaders/day08.ispc";

static const char *solve_fn_name = "solvep1";
using solve_fn_t = i32 (*)(box boxes[], i32 num_boxes, i32 num_iter);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    solve_fn_t solvep1_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    std::vector<box> boxes;
    for(strview box_line : sv_split(input, "\n")) {
        strview lo, rest;
        sv_split_once(box_line, ",", &lo, &rest);

        strview mid, hi;
        sv_split_once(rest, ",", &mid, &hi);

        boxes.emplace_back(atoi(lo.ptr), atoi(mid.ptr), atoi(hi.ptr));
    }

    printf("[DEBUG] Found %lld boxes\n", boxes.size());

    i32 num_conn = 0;
    i32 p1_res = solvep1_fn(boxes.data(), (i32)boxes.size(), ITER);

    print_res(p1, "%i", p1_res);
    print_res(p2, "%i", 0);
    return 0;
}
