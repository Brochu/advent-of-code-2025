#include <ispc.h>

#include "day.hpp"
#include "parse.hpp"
#include "timer.hpp"

char in[] = {
    #include "../inputs/day00.inc"
};
strview input { in, strlen(in) };

static const char *ispc_src = "./shaders/day00.ispc";
static const char *ispc_fn = "sums";

typedef void (*sums_func_t)(int vals[], int starts[], int lens[], int outs[], int count);

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
    int res = result->CompileFromFileToJit(ispc_src);
    if (res != 0) {
        printf("[ISPC] Could not compile code in day00.ispc\n");
        return 1;
    }
    sums_func_t func = (sums_func_t)result->GetJitFunction(ispc_fn);
    printf("[ISPC] Found compiled function from JIT ispc -> 0x%p\n", func);

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

    for (int i = 0; i < lens.back(); i++) {
        printf(" '%i'\n", elves[i + starts.back()]);
    }

    ispc::Shutdown();
    return 0;
}
