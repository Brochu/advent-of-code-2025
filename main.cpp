#include <cstdio>
#include <print>
#include <string_view>

#define ANS_SIZE 256

//TODO: Look into integrating SDL3 lib in order to use GPU to solve problems
//Could also render some solutions visually?

// Day solve function prototype
int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]);

int main(int argc, char **argv) {
    std::string_view sv = {};
    if (argc >= 4 && strcmp(argv[1], "xvert") == 0) {
        std::print("[AoC2025] Convert tool\n");
        std::print("\t - from '{}'\n", argv[2]);
        std::print("\t - to   '{}'\n", argv[3]);

        FILE *f = fopen(argv[2], "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
        std::print("[AoC2025] File size = {}\n", fsize);

        char *content = (char*)malloc(fsize + 1);
        fread(content, fsize, 1, f);
        content[fsize] = 0;
        fclose(f);

        std::print("[AoC2025] Content = \n'{}'\n", content);

        FILE *out = fopen(argv[3], "wb");
        for (size_t i = 0; i <= fsize; i++) {
            fprintf(out, "0x%02x ,", content[i]);

            if (((i+1) % 18) == 0) {
                fprintf(out, "\n");
            }
        }
        fclose(out);

        free(content);
        return 0;
    }
    std::print("[AoC2025] Starting...\n");

    char part1[ANS_SIZE];
    memset(part1, 0, ANS_SIZE);

    char part2[ANS_SIZE];
    memset(part2, 0, ANS_SIZE);
    int res = solve(part1, part2);

    std::println("[AoC2025] Solved {} with {}...", DAY, res);
    std::println("    - Part1 : '{}'", part1);
    std::println("    - Part2 : '{}'", part2);
    return 0;
}
