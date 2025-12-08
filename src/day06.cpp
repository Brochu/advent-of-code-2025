#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
char in[] = {
    #include "../inputs/day06.inc"
};
#else
char in[] = {
"123 328  51 64 \n"
" 45 64  387 23 \n"
"  6 98  215 314\n"
"*   +   *   +  \n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day06.ispc";

static const char *part1_fn_name = "part1";
using part1_fn_t = i64 (*)(i64 nums[], i8 ops[], u64 num_ops, u64 op_len);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "" }, ispc_src);
    part1_fn_t part1_fn = (part1_fn_t)engine->GetJitFunction(part1_fn_name);

    std::vector<i64> nums;
    std::vector<strview> lines = sv_split(input, "\n");

    u64 op_len = lines.size() - 1;
    u64 num_ops = 0;
    for (i32 i = 0; i < op_len; i++) {
        u64 cnt = 0;
        for (strview v : sv_split(lines[i], " ")) {
            if (v.len == 0) { continue; }
            cnt++;

            nums.emplace_back(atoll(v.ptr));
        }
        num_ops = cnt;
    }

    std::vector<i8> ops;
    for (strview op : sv_split(lines.back(), " ")) {
        if (op.len == 0) { continue; }

        ops.emplace_back(op.ptr[0] - '*'); // 0: mult ; 1: add
    }

    i64 total = part1_fn(nums.data(), ops.data(), num_ops, op_len);

    print_res(p1, "%lld", total);
    print_res(p2, "%lld", 0LL);
    return 0;
}
