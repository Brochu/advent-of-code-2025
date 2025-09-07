#include "SDL_gpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <wrl.h>

#include <dxcapi.h>
#include <SDL.h>

#define ANS_SIZE 256

using Microsoft::WRL::ComPtr;

// Day solve function prototype
int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]);

void convert_file(char *from, char *to);
void compile_shader(const char *file_path);

int main(int argc, char **argv) {
    if (argc >= 4 && strcmp(argv[1], "xvert") == 0) {
        convert_file(argv[2], argv[3]);
        return 0;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return 1;
    }
    SDL_GPUDevice *device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, NULL);
    if (device == NULL) {
        printf("[AoC2025] Could not create GPU device: %s\n", SDL_GetError());
        return 1;
    }
    SDL_GPUComputePipelineCreateInfo info { 0 };
    compile_shader("");

    printf("[AoC2025] Starting...\n");

    char part1[ANS_SIZE];
    memset(part1, 0, ANS_SIZE);

    char part2[ANS_SIZE];
    memset(part2, 0, ANS_SIZE);
    int res = solve(part1, part2);

    printf("[AoC2025] Solved %i with %i...\n", DAY, res);
    printf("    - Part1 : '%s'\n", part1);
    printf("    - Part2 : '%s'\n", part2);

    SDL_DestroyGPUDevice(device);
    SDL_Quit();
    return 0;
}

// !! INPUT CONVERTER !!
void convert_file(char *from, char *to) {
    printf("[AoC2025] Convert tool\n");
    printf("\t - from '%s'\n", from);
    printf("\t - to   '%s'\n", to);

    FILE *f = fopen(from, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    printf("[AoC2025] File size = %ld\n", fsize);

    char *content = (char*)malloc(fsize + 1);
    fread(content, fsize, 1, f);
    content[fsize] = 0;
    fclose(f);

    printf("[AoC2025] Content = \n'%s'\n", content);

    FILE *out = fopen(to, "wb");
    for (size_t i = 0; i <= fsize; i++) {
        fprintf(out, "0x%02x ,", content[i]);

        if (((i+1) % 18) == 0) {
            fprintf(out, "\n");
        }
    }
    fclose(out);

    free(content);
}
// -----------------------------------------------------------------------------

// !! SHADER COMPILE !!
void compile_shader(const char *file_path) {
    ComPtr<IDxcUtils> dxc_utils;
    ComPtr<IDxcCompiler3> dxc_comp;

    if (FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc_utils)))) {
        printf("[AoC2025] Could not create instance for DxcUtils\n");
        exit(1);
    }
    if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_comp)))) {
        printf("[AoC2025] Could not create instance for DxcCompiler\n");
        exit(1);
    }

    const char* hlslSource = "[numthreads(8, 1, 1)] void main() { }";
    DxcBuffer buf;
    buf.Ptr = hlslSource;
    buf.Size = strlen(hlslSource);
    buf.Encoding = DXC_CP_ACP;

    const wchar_t *args[] = {
        L"-E", L"main",
        L"-T", L"cs_6_7"
    };
    ComPtr<IDxcResult> result;
    HRESULT hr = dxc_comp->Compile(&buf, args, _countof(args), nullptr, IID_PPV_ARGS(&result));
    if (FAILED(hr)) {
        printf("[AoC2025] Shader compile error with file %s\n", file_path);
        //TODO: Get shader compile error to show
        exit(1);
    }
    ComPtr<IDxcBlobUtf8> err;
    result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&err), nullptr);
    printf("[AoC2025] Shader compile error -> '%.*s'\n", (int)err->GetStringLength(), err->GetStringPointer());

    ComPtr<IDxcBlob> s_blob;
    result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&s_blob), nullptr);
    if (s_blob) {
        printf("[AoC2025] Shader compile completed successfully -> len = %lld\n", s_blob->GetBufferSize());
    }
}
// -----------------------------------------------------------------------------
