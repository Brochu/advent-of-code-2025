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
    auto engine = compile_ispc({ "-g" }, ispc_src);
    step_fn_t step_fn = (step_fn_t)engine->GetJitFunction(step_fn_name);

    std::vector<strview> lines = sv_split(input, "\n");
    i32 height = lines.size();
    i32 width = lines[0].len;

    coord start;
    for(i32 i = 0; i < lines[0].len; i++) {
        if (input.ptr[i] == 'S') {
            start.x = i;
            start.y = 0;
            break;
        }
    }

    std::vector<coord> splitters;
    for (i32 i = 1; i < lines.size(); i++) {
        for (i32 j = 1; j < lines[i].len; j++) {
            if (lines[i].ptr[j] == '^') {
                coord s;
                s.x = j;
                s.y = i;
                splitters.push_back(std::move(s));
            }
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

        for(auto it = dedup.begin(); it != dedup.end(); it++) {
            coord c;
            c.key = *it;
            active.push_back(std::move(c));
            printf(" - new active at (%i, %i) [%llu]\n", c.x, c.y, c.key);
        }
        dedup.clear();
        next.clear();
        next.resize(active.size() * 2);
        num_next = 0;


        printf("DEBUG: calling step: active=%zu next_size=%zu next_capacity=%zu num_next(before)=%d num_splits(before)=%d\n",
               active.size(), next.size(), next.capacity(), num_next, num_splits);

        step_fn(splitters.data(), splitters.size(), height, active.data(), active.size(), next.data(), &num_next, &num_splits);
        printf("DEBUG: returned: num_next=%d num_splits=%d\n", num_next, num_splits);

        // Dump the next[] contents the ISPC wrote
        for (int i = 0; i < num_next; ++i) {
            printf("DEBUG: next[%d] = (%d, %d) [key=%llu]\n", i, next[i].x, next[i].y, next[i].key);
        }

    } while (num_next > 0);

    print_res(p1, "%i", num_splits);
    print_res(p2, "%i", 0);
    return 0;
}
