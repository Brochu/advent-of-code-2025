#include <cstdio>
#include <print>

// Day solve function prototype
int solve(char *p1, char *p2);

int main(int argc, char **argv) {
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

        static const int cols = 18;
        int lines = (fsize/cols) + 1;
        std::println("[AoC2025] lines = {}", lines);

        FILE *out = fopen(argv[3], "wb");
        for (int i = 0; i < lines; i++) {
            for (int j = 0; j < cols; j++) {
                size_t idx = (i*cols) + j;
                char c = (idx >= fsize) ? '\0' : content[idx];
                fprintf(out, "0x%02x ,", c);
            }
            fprintf(out, "\n");
        }
        fclose(out);

        free(content);
        return 0;
    }
    std::print("[AoC2025] Starting...\n");

    char part1[256];
    char part2[256];
    int res = solve(part1, part2);

    std::println("[AoC2025] Solved {} with {}...", DAY, res);
    std::println("    - Part1 : '{}'", part1);
    std::println("    - Part2 : '{}'", part2);
    return 0;
}
