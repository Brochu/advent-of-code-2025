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
    std::vector<i64> xs;
    std::vector<i64> ys;
    std::vector<i64> zs;
};

static const char *ispc_src = "./shaders/day08.ispc";

static const char *solve_fn_name = "solvep1";
using solve_fn_t = i64 (*)(i64 x[], i64 y[], i64 z[], i64 num_boxes, i64 num_iter, i64 from[], i64 to[], i64 dists[], i32 ids[], i32 sizes[]);

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
    const size_t n = b.xs.size();

    printf("[DEBUG] Found %lld boxes\n", b.xs.size());
    std::vector<i64> f_idx, t_idx, dists;
    f_idx.resize((n*(n-1))/2, -1);
    t_idx.resize((n*(n-1))/2, -1);
    dists.resize((n*(n-1))/2, -1);

    std::vector<i32> circ_ids, circ_sizes;
    circ_ids.resize(n);
    circ_sizes.resize(n, 1);
    for (i32 i = 0; i < n; i++) {
        circ_ids[i] = i;
    }

    i64 p1_res = solvep1_fn(
        b.xs.data(), b.ys.data(), b.zs.data(), b.xs.size(),
        ITER,
        f_idx.data(), t_idx.data(), dists.data(),
        circ_ids.data(), circ_sizes.data());

    for (i32 i = 0; i < f_idx.size(); i++) {
        i64 f = f_idx[i];
        i64 t = t_idx[i];
        printf(" - [%lld] From: %lld (%lld, %lld, %lld) -> To: %lld (%lld, %lld, %lld)\n",
           dists[i],
           f, b.xs[f], b.ys[f], b.zs[f],
           t, b.xs[t], b.ys[t], b.zs[t]);
    }

    for (i32 i = 0; i < n; i++) {
        i32 id = circ_ids[i];
        i32 size = circ_sizes[id];

        printf(" - Box #%i : id = %i (size = %i)\n", i, id, size);
    }

    print_res(p1, "%lld", p1_res);
    print_res(p2, "%i", 0);
    return 0;
}
