#include <string_view>
#include <vector>

#include <windows.h>
#include <wrl.h>
#include <dxcapi.h>
#include <SDL.h>

#include "day.hpp"

using Microsoft::WRL::ComPtr;

char in[] = {
    #include "../inputs/in00.3.inc"
};
std::string_view input(in);

extern ComPtr<IDxcBlob> compile_shader(LPCWSTR file_path, LPCWSTR entry, LPCWSTR profile);

//TODO: Find a way to include this from a header file
extern std::string_view sv_strstr(std::string_view haystack, std::string_view needle);
extern std::vector<std::string_view> sv_split(std::string_view str, std::string_view delim);
extern bool sv_split_once(std::string_view str, std::string_view delim,
                          std::string_view* first, std::string_view* second);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto lines = sv_split(input, "\r\n");
    for (std::string_view l : lines) {
        SDL_Log(" - '" SV_FMT "'\n", SV_ARG(l));
    }
    std::string_view left, right;
    if (sv_split_once(lines[2], " ", &left, &right)) {
        SDL_Log(" LEFT: '" SV_FMT "'\n", SV_ARG(left));
        SDL_Log(" RIGHT: '" SV_FMT "'\n", SV_ARG(right));
    }

    print_res(p1, "Hello -> %i", 69);
    print_res(p2, "Hello -> %i", 420);

    //ComPtr<IDxcBlob> shader = compile_shader(L"", L"main", L"cs_6_7");
    /*
    SDL_Window *window;
    SDL_Renderer *ctx;
    SDL_CreateWindowAndRenderer("[DAY00] Main Window", 800, 600, 0, &window, &ctx);

    bool quit = false;
    SDL_Event evt;
    while(!quit) {
        while(SDL_PollEvent(&evt)) {
            if (evt.type == SDL_EVENT_QUIT) {
                quit = true;
                break;
            }

            SDL_SetRenderDrawColor(ctx, 0, 0, 0, 0);
            SDL_RenderClear(ctx);

            SDL_SetRenderDrawColor(ctx, 255, 255, 0, 0);
            SDL_FRect rect { 0, 0, 100, 100 };
            SDL_RenderFillRect(ctx, &rect);

            SDL_RenderPresent(ctx);
        }
    }

    SDL_DestroyRenderer(ctx);
    SDL_DestroyWindow(window);
    */
    return 0;
}
