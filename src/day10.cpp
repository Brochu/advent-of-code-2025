#include <ispc.h>
#include <span>
#include <queue>

#include "day.hpp"
#include "parse.hpp"
#include "simd.hpp"

#if 1
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
using solve_fn_t = i32 (*)();

#define MAX_BUTTONS 32
#define MAX_JOLTAGES 32
struct machine_t {
    i16 target;
    i16 buttons[MAX_BUTTONS];
    i16 joltages[MAX_JOLTAGES];

    i8 num_buttons = 0;
    i8 num_joltages = 0;
};
void machine_parse(machine_t &m, strview line) {
    i16 temp = 1;
    for (i32 i = 0; i < line.len; i++) {
        if (line.ptr[i] == '.') {
            temp <<= 1;
        }
        else if (line.ptr[i] == '#') {
            m.target |= temp;
            temp <<= 1;
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
        else if (line.ptr[i] == '{') {
            i32 j = i;
            while (line.ptr[j] != '}') j++;
            for (strview v : sv_split({ &line.ptr[i+1], (u64)(j-i-1) }, ",")) {
                m.joltages[m.num_joltages++] = atoi(v.ptr);
            }
            i = j+1;
        }
    }
}
void machine_print(std::span<machine_t> ms) {
    for (machine_t &m : ms) {
        printf("# target = %i\n", m.target);
        for (int i = 0; i < m.num_buttons; i++) {
            printf(" - b = %i\n", m.buttons[i]);
        }
        for (int i = 0; i < m.num_joltages; i++) {
            printf(" * j = %i\n", m.joltages[i]);
        }
    }
}

struct state_t {
    machine_t *machine;

    union {
        struct {
            i16 target;
            i16 current;
            i16 last_button;
            i16 padding;
        };
        u64 key;
    };
    i32 depth;
};
state_t state_init(machine_t &m) {
    state_t ret;
    ret.machine = &m;
    ret.target = m.target;
    ret.current = 0;
    ret.last_button = -1;
    ret.padding = 0xFFF;
    ret.depth = 0;
    return ret;
}
void state_print(std::span<state_t> states) {
    for (state_t &s : states) {
        printf("[STATE][key=%llu] target=%i vs. current=%i (last=%i)\n", s.key, s.target, s.current, s.last_button);
    }
}

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto engine = compile_ispc({ "-g" }, ispc_src);
    solve_fn_t solve_fn = (solve_fn_t)engine->GetJitFunction(solve_fn_name);

    std::vector<strview> lines = sv_split(input, "\n");

    std::vector<machine_t> machines;
    std::vector<i32> results;
    machines.resize(lines.size());
    results.resize(lines.size());
    for (i32 i = 0; i < lines.size(); i++) {
        machine_parse(machines[i], lines[i]);
    }

    for (i32 machine_idx = 0; machine_idx < machines.size(); machine_idx++) {
        std::queue<state_t> Q;
        Q.push(state_init(machines[machine_idx]));

        for (; !Q.empty(); Q.pop()) {
            state_t &s = Q.front();

            if (s.target == s.current) {
                results[machine_idx] = s.depth;
                printf("[DEBUG] Result[%i] = %i\n", machine_idx, s.depth);
                break;
            }

            for (i8 j = 0; j < s.machine->num_buttons; j++) {
                if (j == s.last_button) {
                    continue;
                }
                state_t next = state_init(*s.machine);
                next.current = s.current ^ s.machine->buttons[j];
                next.last_button = j;
                next.depth = s.depth+1;

                Q.push(next);
            }
        }
    }

    i32 p1_res = 0;
    for (i32 res : results) {
        p1_res += res;
    }

    print_res(p1, "%i", p1_res);
    print_res(p2, "%i", 0);
    return 0;
}
