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

static const char *solve_fn_name = "solve";
using solve_fn_t = i64 (*)(i64 nums[], i8 ops[], u64 num_ops, u64 op_len);

i64 parse_num(strview line, i64 base, i64 end) {
    i64 val = 0;
    for (i32 j = 0; j < (end-base); j++) {
        char curr = line.ptr[base+j];
        if (curr != ' ') { val = (val*10) + (curr - '0'); }
    }

    return val;
}

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "" }, ispc_src);
    solve_fn_t solve_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    std::vector<strview> lines = sv_split(input, "\n");
    strview &op_line = lines.back();
    std::vector<i8> ops_p1;
    std::vector<i64> nums_p1;

    i64 base = 0;
    i64 end;
    for (end = 1; end < op_line.len; end++) {
        if (op_line.ptr[end] != ' ') {
            for (i32 i = 0; i < lines.size()-1; i++) {
                nums_p1.emplace_back(parse_num(lines[i], base, end));
            }
            ops_p1.emplace_back(op_line.ptr[base] - '*');
            base = end;
        }
    }

    for (i32 i = 0; i < lines.size()-1; i++) {
        nums_p1.emplace_back(parse_num(lines[i], base, end));
    }
    ops_p1.emplace_back(op_line.ptr[base] - '*');

    std::vector<i8> ops_p2;
    std::vector<i64> nums_p2;
    base = op_line.len;
    for (end = base - 1; end >= 0; end--) {
        if (op_line.ptr[end] != ' ') {
            for (i32 j = 0; j < (base-end); j++) {
                i64 val = 0;
                for (i32 i = 0; i < lines.size()-1; i++) {
                    char c = lines[i].ptr[base-1-j];
                    if (c != ' ') {
                        val = (val*10) + (c - '0');
                    }
                }
                nums_p2.emplace_back(val);
            }
            ops_p2.emplace_back(op_line.ptr[end] - '*');
            for (i32 i = 0; i < 4 - (base-end); i++) {
                nums_p2.emplace_back(0); // PADDING
            }
            base = end - 1;
        }
    }

    i64 total_p1 = solve_fn(nums_p1.data(), ops_p1.data(), ops_p1.size(), lines.size()-1);
    i64 total_p2 = solve_fn(nums_p2.data(), ops_p2.data(), ops_p2.size(), 4);

    print_res(p1, "%lld", total_p1);
    print_res(p2, "%lld", total_p2);
    return 0;
}
