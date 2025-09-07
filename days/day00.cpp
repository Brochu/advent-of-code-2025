#include <string>

#include <windows.h>
#include <wrl.h>
#include <dxcapi.h>
#include <SDL.h>

#include "day.hpp"

using Microsoft::WRL::ComPtr;

char in[] = {
    #include "../inputs/in00.3.inc"
};
std::string input(in);

extern ComPtr<IDxcBlob> compile_shader(LPCWSTR file_path, LPCWSTR entry, LPCWSTR profile);

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    SDL_Log("[Day00] input: ins/in00.txt\n");

    SDL_Log("Content:\n'%s'\n", input.c_str());
    std::string_view view = { in + 4, 5 };
    SDL_Log("View: '" SV_FMT "'\n", SV_ARG(view));

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
