#include <cstring>
#include <ispc.h>
#include <assert.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day11.inc"
};
#else
char in[] = {
"aaa: you hhh\n"
"you: bbb ccc\n"
"bbb: ddd eee\n"
"ccc: ddd eee fff\n"
"ddd: ggg\n"
"eee: out\n"
"fff: out\n"
"ggg: out\n"
"hhh: ccc fff iii\n"
"iii: out\n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day11.ispc";

static const char *solve_fn_name = "solve";
using solve_fn_t = i32 (*)();

struct label_t {
    union {
        struct {
            i8 pad0;
            i8 text[3];
        };
        u32 key = 0;
    };
};
label_t label_make(const char *lbl) {
    assert(strlen(lbl) >= 3);

    label_t l;
    l.pad0 = 0xFF;
    memcpy_s(l.text, 3, lbl, 3);
    return l;
};
void label_print(label_t l) {
    printf("[LABEL] text = %c%c%c; key = %u\n", l.text[0], l.text[1], l.text[2], l.key);
}

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    solve_fn_t solve_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    printf("%s", in);
    label_t lbl = label_make("you");
    label_print(lbl);

    print_res(p1, "%i", 0);
    print_res(p2, "%i", 0);
    return 0;
}
