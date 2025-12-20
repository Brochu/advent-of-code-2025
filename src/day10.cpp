#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 0
char in[] = {
    #include "../inputs/day10.inc"
};
#else
char in[] = {
"[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}\n"
"[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}\n"
"[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}\n"
};
#endif
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day10.ispc";

static const char *solve_fn_name = "solve";
using solve_fn_t = i64 (*)(i64 xs[], i64 ys[], i32 len);

#define MAX_BUTTONS 32
#define MAX_JOLTAGES 32
struct machine {
    i16 target;
    i16 buttons[MAX_BUTTONS];
    i16 joltages[MAX_JOLTAGES];

    i8 num_buttons = 0;
    i8 num_joltages = 0;
};
void machine_parse(machine &m, strview line) {
    for (i32 i = 0; i < line.len; i++) {
        if (line.ptr[i] == '.') {
            m.target <<= 1;
        }
        else if (line.ptr[i] == '#') {
            m.target <<= 1;
            m.target |= 1;
        }
        else if (line.ptr[i] == '(') {
            i32 j = i;
            while (line.ptr[j] != ')') j++;
            i16 btn = 0;
            for (strview v : sv_split({ &line.ptr[i+1], (u64)(j-i-1) }, ",")) {
                btn |= 1 << atoi(v.ptr);
            }
            m.buttons[m.num_buttons++] = btn;
            i = j+1;
        }
    }
}

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    solve_fn_t solve_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    std::vector<strview> lines = sv_split(input, "\n");

    std::vector<machine> machines;
    machines.resize(lines.size());
    for (i32 i = 0; i < lines.size(); i++) {
        machine_parse(machines[i], lines[i]);
    }

    for (machine &m : machines) {
        printf("# target = %i\n", m.target);
        for (int i = 0; i < m.num_buttons; i++) {
            printf(" - b = %i\n", m.buttons[i]);
        }
    }

    print_res(p1, "%i", 0);
    print_res(p2, "%i", 0);
    return 0;
}
