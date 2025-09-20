#pragma once

#include <assert.h>
#include <windows.h>
#include <wrl.h>

#include <dxcapi.h>

#define CHECK(hr) assert(SUCCEEDED(hr));

Microsoft::WRL::ComPtr<IDxcBlob> compile_shader(LPCWSTR file_path, LPCWSTR entry, LPCWSTR profile);
