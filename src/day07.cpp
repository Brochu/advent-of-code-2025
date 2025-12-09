#include <ispc.h>
#include <set>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day07.inc"
};
#else
char in[] = {
".......S.......\n"
"...............\n"
".......^.......\n"
"...............\n"
"......^.^......\n"
"...............\n"
".....^.^.^.....\n"
"...............\n"
"....^.^...^....\n"
"...............\n"
"...^.^...^.^...\n"
"...............\n"
"..^...^.....^..\n"
"...............\n"
".^.^.^.^.^...^.\n"
"...............\n"
};
#endif
strview input { in, strlen(in) };

struct coord {
    union {
        struct { i32 x; i32 y; };
        u64 key;
    };
};

static const char *ispc_src = "./shaders/day07.ispc";

static const char *step_fn_name = "step";
using step_fn_t = void (*)(coord splitters[], i32 num_splitters, i32 max_height, coord active[], i32 num_active, coord next[], i32 *num_next, i32 *num_splits);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "" }, ispc_src);
    step_fn_t step_fn = (step_fn_t)engine->GetJitFunction(step_fn_name);

    std::vector<strview> lines = sv_split(input, "\n");
    coord start;
    for(i32 i = 0; i < input.len; i++) {
        if (input.ptr[i] == 'S') {
            start.x = i;
            start.y = 0;
            break;
        }
    }

    std::set<u64> dedup;
    std::vector<coord> active;
    std::vector<coord> next;
    next.push_back(std::move(start));

    i32 num_next = 1;
    i32 num_splits = 0;

    do {
        for(i32 i = 0; i < num_next; i++) {
            dedup.insert(next[i].key);
        }
        active.clear();

        next.clear();
        next.resize(active.size() * 2);
        num_next = 0;

        for(auto it = dedup.begin(); it != dedup.end(); it++) {
            coord c;
            c.key = *it;
            active.push_back(std::move(c));
            printf(" - new active at (%i, %i) [%llu]\n", c.x, c.y, c.key);
        }
        dedup.clear();

        step_fn(nullptr, 0, lines.size(), active.data(), active.size(), next.data(), &num_next, &num_splits);
        printf(" - num_splits = %i\n", num_splits);
    } while (num_next > 0);

    print_res(p1, "%lld", 0LL);
    print_res(p2, "%lld", 0LL);
    return 0;
}
