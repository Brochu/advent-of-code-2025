#pragma once

#include <windows.h>
#include <wrl.h>

#include <dxcapi.h>

Microsoft::WRL::ComPtr<IDxcBlob> compile_shader(LPCWSTR file_path, LPCWSTR entry, LPCWSTR profile);
