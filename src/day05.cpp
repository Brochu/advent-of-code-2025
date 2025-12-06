#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
char in[] = {
    #include "../inputs/day05.inc"
};
#else
char in[] = {
"3-5\n"
"10-14\n"
"16-20\n"
"12-18\n"
"\n"
"1\n"
"5\n"
"8\n"
"11\n"
"17\n"
"32\n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day05.ispc";

static const char *part1_fn_name = "part1";
using part1_fn_t = i64 (*)(i64 ls[], i64 hs[], i64 num_ranges, i64 ids[], i64 num_ids);

static const char *part2_fn_name = "part2";
using part2_fn_t = i64 (*)(i64 ls[], i64 hs[], i64 num_ranges);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    part1_fn_t part1_fn = (part1_fn_t)engine->GetJitFunction(part1_fn_name);
    part2_fn_t part2_fn = (part2_fn_t)engine->GetJitFunction(part2_fn_name);

    strview r_str, i_str;
    sv_split_once(input, "\n\n", &r_str, &i_str);

    std::vector<i64> ls;
    std::vector<i64> hs;
    for (strview range : sv_split(r_str, "\n")) {
        strview lo, hi;
        sv_split_once(range, "-", &lo, &hi);
        ls.emplace_back(atoll(lo.ptr));
        hs.emplace_back(atoll(hi.ptr));
    }

    std::vector<i64> ids;
    for (strview i : sv_split(i_str, "\n")) {
        ids.emplace_back(atoll(i.ptr));
    }

    i64 value_p1 = part1_fn(ls.data(), hs.data(), ls.size(), ids.data(), ids.size());
    i64 value_p2 = part2_fn(ls.data(), hs.data(), ls.size());

    print_res(p1, "%lld", value_p1);
    print_res(p2, "%lld", value_p2);
    return 0;
}
