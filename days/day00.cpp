#include <stdio.h>
#include <string>
#include <print>

char in[] = {
    #include "../ins/in00.inc"
};
std::string input(in);

int solve(char *p1, char *p2) {
    printf("[Day00] input: ins/in00.txt\n");

    printf("Content:\n'%s'\n", input.c_str());
    std::string_view view = { in + 4, 5 };
    std::println("View: {}", view);

    strcpy_s(p1, 256, "Hello");
    strcpy_s(p2, 256, "World");
    return 69;
}
