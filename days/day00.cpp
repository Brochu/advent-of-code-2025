#include <stdio.h>
#include <string>

char in[] = {
    #include "../inputs/in00.new.inc"
};
std::string input(in);

int solve(char p1[256], char p2[256]) {
    printf("[Day00] input: ins/in00.txt\n");

    printf("Content:\n'%s'\n", input.c_str());
    std::string_view view = { in + 4, 5 };
    printf("View: '%.*s'\n", (int)view.length(), view.data());

    strcpy_s(p1, 256, "Hello");
    strcpy_s(p2, 256, "World");
    return 69;
}
