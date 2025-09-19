#include <stdio.h>
#include <string.h>

#include "day.hpp"
#include "parse.hpp"
#include "hlsl.hpp"
using Microsoft::WRL::ComPtr;

void convert_file(const char *from, const char *to);

int main(int argc, char **argv) {
    if (argc >= 4 && strcmp(argv[1], "xvert") == 0) {
        convert_file(argv[2], argv[3]);
        return 0;
    }

    char part1[ANS_SIZE];
    memset(part1, 0, ANS_SIZE);

    char part2[ANS_SIZE];
    memset(part2, 0, ANS_SIZE);

    int res = solve(part1, part2);
    printf("[AoC2025] Solved %i with code %i\n", DAY, res);
    printf("    - Part1 : '%s'\n", part1);
    printf("    - Part2 : '%s'\n", part2);

    return 0;
}

// !! INPUT CONVERTER !!
void convert_file(const char *from, const char *to) {
    printf("[AoC2025] Convert tool | '%s' -> '%s'\n", from, to);

    FILE *f;
    fopen_s(&f, from, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    rewind(f);

    char *content = (char *)malloc(fsize + 1);
    fread_s(content, fsize, 1, fsize, f);
    content[fsize] = '\0';
    printf("[AoC2025] fsize = %ld\n", fsize);
    printf("[AoC2025] Content = \n'%s'\n", content);

    fclose(f);

    fopen_s(&f, to, "wb");
    static int cols = 18;

    for (size_t i = 0; i <= fsize; i++) {
        fprintf_s(f, "0x%02x,", content[i]);

        if (((i+1) % cols) == 0) {
            fprintf_s(f, "\n");
        }
    }

    fclose(f);
    free(content);
}
// -----------------------------------------------------------------------------

// !! SHADER COMPILE !!
ComPtr<IDxcBlob> compile_shader(LPCWSTR file_path, LPCWSTR entry, LPCWSTR profile) {
    ComPtr<IDxcUtils> dxc_utils;
    ComPtr<IDxcCompiler3> dxc_comp;
    ComPtr<IDxcContainerReflection> dxc_refl;

    if (FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc_utils)))) {
        printf("[AoC2025] Could not create instance for DxcUtils\n");
        exit(1);
    }
    if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_comp)))) {
        printf("[AoC2025] Could not create instance for DxcCompiler\n");
        exit(1);
    }
    if (FAILED(DxcCreateInstance(CLSID_DxcContainerReflection, IID_PPV_ARGS(&dxc_refl)))) {
        printf("[AoC2025] Could not create instance for DxcContainerReflection\n");
        exit(1);
    }

    ComPtr<IDxcBlobEncoding> hlslsrc;
    dxc_utils->LoadFile(file_path, NULL, &hlslsrc);
    DxcBuffer buf;
    buf.Ptr = hlslsrc->GetBufferPointer();
    buf.Size = hlslsrc->GetBufferSize();
    buf.Encoding = DXC_CP_ACP;

    const wchar_t *args[] = {
        L"-E", entry,
        L"-T", profile
    };

    ComPtr<IDxcResult> result;
    HRESULT hr = dxc_comp->Compile(&buf, args, _countof(args), nullptr, IID_PPV_ARGS(&result));
    result->GetStatus(&hr);
    if (FAILED(hr)) {
        printf("[AoC2025] Shader compile error with file '%ls'\n", file_path);

        ComPtr<IDxcBlobUtf8> err;
        result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&err), nullptr);
        printf("[AoC2025] Shader compile error -> '%.*s'\n", (int)err->GetStringLength()-1, err->GetStringPointer());

        exit(1);
    }

    ComPtr<IDxcBlob> s_blob;
    result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&s_blob), nullptr);
    if (s_blob) {
        printf("[AoC2025] Shader compile completed successfully -> len = %lld\n", s_blob->GetBufferSize());
        return s_blob;
    }
    return nullptr;
}
// -----------------------------------------------------------------------------
// !! STRING MANIPULATION !!
strview sv_strstr(strview haystack, strview needle) {
    const char *found = strstr(haystack.ptr, needle.ptr);
    size_t pos = found - haystack.ptr;
    if (pos >= haystack.len) {
        return { "\0", 0 };
    }
    return { found, needle.len };
}

std::vector<strview> sv_split(strview str, strview delim) {
    std::vector<strview> result;
    size_t pos = 0;

    while (pos < str.len) {
        strview start(str.ptr + pos, str.len - pos);
        size_t end = (sv_strstr(start, delim).ptr - str.ptr);
        if (end >= str.len) {
            result.push_back({ str.ptr + pos, str.len - pos });
            break;
        }
        result.push_back({ str.ptr + pos, end - pos });
        pos = end + delim.len; // skip over the whole delimiter
    }
    return result;
}

bool sv_split_once(strview str, strview delim, strview* first, strview* second) {
    size_t pos = (sv_strstr(str, delim).ptr - str.ptr);
    if (pos >= str.len) {
        return false; // delimiter not found
    }

    first->ptr = str.ptr;
    first->len = pos;
    second->ptr = str.ptr + pos + delim.len;
    second->len = str.len - pos - delim.len;
    return true;
}
// -----------------------------------------------------------------------------
