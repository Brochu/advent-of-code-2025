#include "day.hpp"
#include "parse.hpp"
#include "hlsl.hpp"
using Microsoft::WRL::ComPtr;

char in[] = {
    #include "../inputs/in00.4.inc"
};
strview input { in, strlen(in) };

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
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

    ComPtr<IDxcBlob> shader = compile_shader(L"./shaders/day00.hlsl", L"main", L"cs_6_5");
    return 0;
}
