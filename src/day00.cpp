#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "timer.hpp"

char in[] = {
    #include "../inputs/in00.4.inc"
};
strview input { in, strlen(in) };

typedef void (*simple_func_t)(float input[], float output[], int count);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    if (!ispc::Initialize()) {
        printf("[ISPC] Could not init ISPC\n");
        return 1;
    }
    std::vector<std::string> args = {
        //"--target=host",           // Target specification
        //"-O2",                     // Optimization level
    };
    std::unique_ptr<ispc::ISPCEngine> result = ispc::ISPCEngine::CreateFromArgs(args);
    int res = result->CompileFromFileToJit("./shaders/day00.ispc");
    if (res != 0) {
        printf("[ISPC] Could not compile code in day00.ispc\n");
        return 1;
    }
    simple_func_t func = (simple_func_t)result->GetJitFunction("simple");
    printf("[ISPC] Found compiled function from JIT ispc -> 0x%p\n", func);

    uint64_t start = GetPerfCounter();
    const int SIZE = 32;
    float in[SIZE];
    float out[SIZE];
    for (int i = 0; i < SIZE; i++) {
        in[i] = (float)i;
    }
    func(in, out, SIZE);
    for (int i = 0; i < SIZE; i++) {
        printf("[ISPC] Result at %i = %f\n", i, out[i]);
    }
    uint64_t end = GetPerfCounter();
    double time = GetElapsedTime(start, end);
    printf("[ISPC] Solved in = %f\n", time);
    /*
    auto lines = sv_split(input, "\r\n");
    for (strview l : lines) {
        printf("[NW SV] - '" SV_FMT "'\n", SV_ARG(l));
    }

    strview l, r;
    if (sv_split_once(lines[2], " ", &l, &r)) {
        printf(" NEW LEFT: '" SV_FMT "'\n", SV_ARG(l));
        printf(" NEW RIGHT: '" SV_FMT "'\n", SV_ARG(r));
    }

    print_res(p1, "Hello -> %i", 69);
    print_res(p2, "Hello -> %i", 420);
    */

    ispc::Shutdown();
    return 0;
}
