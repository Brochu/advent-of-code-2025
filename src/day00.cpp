#include <algorithm>
#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"
#include "timer.hpp"

char in[] = {
    #include "../inputs/day00.inc"
};
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day00.ispc";

static const char *ispc_fn = "sums";
using sums_func_t = void (*)(int vals[], int starts[], int lens[], int outs[], int count);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "", "" /*"--target=host", "-O2"*/ }, ispc_src);
    sums_func_t sums = (sums_func_t)engine->GetJitFunction(ispc_fn);
    sums_func_t sums_gg = (sums_func_t)engine->GetJitFunction("sums_gang_groups");
    printf("[ISPC] Found compiled function from JIT ispc -> 0x%p\n", sums);
    printf("[ISPC] Found compiled function from JIT ispc -> 0x%p\n", sums_gg);

    std::vector<int> elves;
    std::vector<int> starts;
    starts.emplace_back(0);
    std::vector<int> lens;

    std::vector<strview> groups = sv_split(input, "\n");
    int group_len = 0;
    char buf[32];
    for (strview &g : groups) {
        memcpy_s(buf, 32, g.ptr, g.len);
        buf[g.len] = '\0';
        int value = atoi(buf);

        //printf(" - %i\n", value);
        if (value > 0) {
            elves.emplace_back((int)value);
            group_len++;
        } else {
            lens.emplace_back((int)group_len);
            group_len = 0;
            starts.emplace_back((int)elves.size());
        }
    }
    lens.emplace_back(group_len);
    printf(" - %lld total elves\n", elves.size());
    printf(" - %lld starts\n", starts.size());
    printf(" - %lld lens\n", lens.size());

    std::vector<int> outs;
    outs.resize(starts.size());

    uint64_t start = GetPerfCounter();
    sums(elves.data(), starts.data(), lens.data(), outs.data(), starts.size());
    uint64_t end = GetPerfCounter();
    double time = GetElapsedTime(start, end);
    printf(" -> Time spent in ISPC = %lf\n", time);

    int max = 0;
    for (int i = 0; i < starts.size(); i++) {
        if (outs[i] > max) max = outs[i];
    }

    std::make_heap(outs.begin(), outs.end());

    std::pop_heap(outs.begin(), outs.end());
    int first = outs.back();
    outs.pop_back();

    std::pop_heap(outs.begin(), outs.end());
    int second = outs.back();
    outs.pop_back();

    std::pop_heap(outs.begin(), outs.end());
    int third = outs.back();
    outs.pop_back();

    print_res(p1, "%i", first);
    print_res(p2, "%i", first + second + third);
    return 0;
}
