#include <algorithm>
#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
char in[] = {
    #include "../inputs/day08.inc"
};
const static i32 num_connection = 1000;
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
const static i32 num_connection = 10;
#endif
strview input { in, strlen(in) };

struct boxes {
    std::vector<i32> xs;
    std::vector<i32> ys;
    std::vector<i32> zs;

    std::vector<i32> c_ids;
    std::vector<i32> c_sizes;
};

struct combos {
    std::vector<i32> from;
    std::vector<i32> to;
    std::vector<i64> dist;
};

i32 pivot(combos &c, i32 lo, i32 hi) {
    i64 pivot = c.dist[hi];
    i32 i = lo;

    for (i32 j = lo; j < hi; j++) {
        if (c.dist[j] <= pivot) {
            std::swap(c.dist[j], c.dist[i]);
            std::swap(c.from[j], c.from[i]);
            std::swap(c.to[j], c.to[i]);
            i++;
        }
    }

    std::swap(c.dist[i], c.dist[hi]);
    std::swap(c.from[i], c.from[hi]);
    std::swap(c.to[i], c.to[hi]);
    return i;
}

void quicksort(combos &c, i32 lo, i32 hi) {
    if (lo >= hi || lo < 0) {
        return;
    }

    i32 p = pivot(c, lo, hi);

    quicksort(c, lo, p-1);
    quicksort(c, p+1, hi);
}

static const char *ispc_src = "./shaders/day08.ispc";

static const char *dist_fn_name = "dist";
using dist_fn_t = void (*)(i32 x[], i32 y[], i32 z[], i32 num_boxes, i32 from[], i32 to[], i64 dists[]);

static const char *cnnx_fn_name = "connect";
using cnnx_fn_t = i64 (*)(i32 from[], i32 to[], i32 num_connect, i32 c_ids[], i32 c_sizes[], i32 num_boxes);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    dist_fn_t dist_fn = (dist_fn_t)engine->GetJitFunction(dist_fn_name);
    cnnx_fn_t cnnx_fn = (cnnx_fn_t)engine->GetJitFunction(cnnx_fn_name);

    boxes b;
    std::vector<strview> box_lines = sv_split(input, "\n");
    for(i32 i = 0; i < box_lines.size(); i++) {
        strview lo, rest;
        sv_split_once(box_lines[i], ",", &lo, &rest);

        strview mid, hi;
        sv_split_once(rest, ",", &mid, &hi);

        b.xs.emplace_back(atoi(lo.ptr));
        b.ys.emplace_back(atoi(mid.ptr));
        b.zs.emplace_back(atoi(hi.ptr));

        b.c_ids.emplace_back(i);
        b.c_sizes.emplace_back(1);
    }
    const size_t n = b.xs.size();

    printf("[DEBUG] Found %lld boxes\n", n);
    combos c;
    c.from.resize(n*(n-1)/2, -1);
    c.to.resize(n*(n-1)/2, -1);
    c.dist.resize(n*(n-1)/2, 0);

    dist_fn(b.xs.data(), b.ys.data(), b.zs.data(), n, c.from.data(), c.to.data(), c.dist.data());
    quicksort(c, 0, c.dist.size());

    /*
    for (i32 i = 0; i < (n*(n-1)/2); i++) {
        printf(" - (%i) -- (%i) / {%lli}\n", c.from[i], c.to[i], c.dist[i]);
    }
    */

    i64 res_p1 = cnnx_fn(c.from.data(), c.to.data(), num_connection, b.c_ids.data(), b.c_sizes.data(), n);

    for (i32 i = 0; i < n; i++) {
        printf(" - [(%i, %i, %i)] : ID = %i; SIZE = %i\n", b.xs[i], b.ys[i], b.zs[i], b.c_ids[i], b.c_sizes[i]);
    }

    //i64 res_p1 = 0;
    i64 res_p2 = 0;
    print_res(p1, "%lli", res_p1);
    print_res(p2, "%lli", res_p2);
    return 0;
}
