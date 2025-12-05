#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
char in[] = {
    #include "../inputs/day03.inc"
};
#else
char in[] = {
"987654321111111\n"
"811111111111119\n"
"234234234234278\n"
"818181911112111\n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day03.ispc";

static const char *ispc_fn = "solve";
using solve_func_t = i64 (*)(i8 batteries[], u64 banks[], u64 num_banks, u64 bank_len, i8 k);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);
    solve_func_t solve_fn = (solve_func_t)engine->GetJitFunction(ispc_fn);

    std::vector<i8> batteries;
    std::vector<u64> banks;
    u64 num_banks = 0;
    u64 bank_len = 0;
    for(strview &s : sv_split(input, "\n")) {
        bank_len = s.len;
        banks.emplace_back(bank_len * num_banks);
        num_banks++;

        for (int i = 0; i < s.len; i++) {
            batteries.emplace_back(s.ptr[i] - '0');
        }
    }

    i64 resp1 = solve_fn(batteries.data(), banks.data(), num_banks, bank_len, 2);
    i64 resp2 = solve_fn(batteries.data(), banks.data(), num_banks, bank_len, 12);

    print_res(p1, "%lld", resp1);
    print_res(p2, "%lld", resp2);
    return 0;
}
