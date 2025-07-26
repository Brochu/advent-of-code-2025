#include <stdio.h>

// Day solve function prototype
int solve(char *p1, char *p2);

int main(int argc, char **argv) {
    printf("[AoC2025] Starting...\n");
    char part1[256];
    char part2[256];

    int res = solve(part1, part2);

    printf("[AoC2025] Solved %i with %i...\n", DAY, res);
    printf("    - Part1 : '%s'\n", part1);
    printf("    - Part2 : '%s'\n", part2);
    return 0;
}
