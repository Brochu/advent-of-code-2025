#include <SDL.h>
#include <cstdint>

#include "SDL_gpu.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "day.hpp"
#include "parse.hpp"
#include "hlsl.hpp"
using Microsoft::WRL::ComPtr;

char in[] = {
    #include "../inputs/in00.3.inc"
};
strview input { in, strlen(in) };

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]) {
    auto lines = sv_split(input, "\r\n");
    for (strview l : lines) {
        SDL_Log("[NW SV] - '" SV_FMT "'\n", SV_ARG(l));
    }

    strview l, r;
    if (sv_split_once(lines[2], " ", &l, &r)) {
        SDL_Log(" NEW LEFT: '" SV_FMT "'\n", SV_ARG(l));
        SDL_Log(" NEW RIGHT: '" SV_FMT "'\n", SV_ARG(r));
    }

    print_res(p1, "Hello -> %i", 69);
    print_res(p2, "Hello -> %i", 420);

    // -----

    // EXAMPLE: Running compute shader on GPU
    ComPtr<IDxcBlob> shader = compile_shader(L"./shaders/day00.hlsl", L"main", L"cs_6_5");

    SDL_GPUDevice *device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_DXIL, true, NULL);
    if (!device) {
        SDL_Log("[SDL] Could not create GPU device : %s", SDL_GetError());
        return 1;
    }
    SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(device);
    if (!cmd) {
        SDL_Log("[SDL] Could not acquire command buffer : %s", SDL_GetError());
        return 1;
    }

    SDL_GPUComputePipelineCreateInfo info { 0 };
    info.code = (Uint8*)shader->GetBufferPointer();
    info.code_size = shader->GetBufferSize();
    info.format = SDL_GPU_SHADERFORMAT_DXIL;
    info.threadcount_x = 16;
    info.threadcount_y = 1;
    info.threadcount_z = 1;
    info.num_readwrite_storage_buffers = 1;
    SDL_GPUComputePipeline *pipeline = SDL_CreateGPUComputePipeline(device, &info);
    if (!pipeline) {
        SDL_Log("[SDL] Could not create compute pipeline : %s", SDL_GetError());
        return 1;
    }

    SDL_GPUBufferCreateInfo binfo = {};
    binfo.props = 0;
    binfo.usage = SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE;
    binfo.size = sizeof(uint32_t) * 16;
    SDL_GPUBuffer *buf = SDL_CreateGPUBuffer(device, &binfo);
    SDL_GPUStorageBufferReadWriteBinding bbind;
    bbind.buffer = buf;
    bbind.cycle = false;

    SDL_GPUTransferBufferCreateInfo tbinfo = {};
    tbinfo.size = sizeof(uint32_t) * 16;
    tbinfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
    tbinfo.props = 0;
    SDL_GPUTransferBuffer *target = SDL_CreateGPUTransferBuffer(device, &tbinfo);

    //SDL_GPUTextureCreateInfo tinfo = {};
    //SDL_GPUTexture *tex = SDL_CreateGPUTexture(device, &tinfo);
    //SDL_GPUStorageTextureReadWriteBinding tbind;

    SDL_GPUComputePass *pass = SDL_BeginGPUComputePass(cmd, NULL, 0, &bbind, 1);
    if (!pass) {
        SDL_Log("[SDL] Could not begin compute pass : %s", SDL_GetError());
        return 1;
    }
    SDL_BindGPUComputePipeline(pass, pipeline);
    SDL_DispatchGPUCompute(pass, 1, 1, 1);
    SDL_EndGPUComputePass(pass);

    SDL_GPUCopyPass * cpy = SDL_BeginGPUCopyPass(cmd);
    SDL_GPUBufferRegion src;
    src.buffer = buf;
    src.offset = 0;
    src.size = sizeof(uint32_t) * 16;
    SDL_GPUTransferBufferLocation dst = {};
    dst.transfer_buffer = target;
    dst.offset = 0;
    SDL_DownloadFromGPUBuffer(cpy, &src, &dst);
    SDL_EndGPUCopyPass(cpy);

    SDL_GPUFence *fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
    if (!fence) {
        SDL_Log("[SDL] Could not submit work to GPU : %s", SDL_GetError());
        return 1;
    }
    if (!SDL_WaitForGPUFences(device, 1, &fence, 1)) {
        SDL_Log("[SDL] Wait on fence for GPU work failed : %s", SDL_GetError());
        return 1;
    }
    SDL_ReleaseGPUFence(device, fence);
    SDL_ReleaseGPUComputePipeline(device, pipeline);

    uint32_t *data = (uint32_t*)SDL_MapGPUTransferBuffer(device, target, false);
    for (int i = 0; i < 16; i++) {
        SDL_Log(" - Trying to get data from GPU = %i\n", data[i]);
    }
    SDL_UnmapGPUTransferBuffer(device, target);

    SDL_DestroyGPUDevice(device);

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
