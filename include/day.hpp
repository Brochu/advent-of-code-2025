#pragma once

#define ANS_SIZE 256

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)sv.len, sv.ptr
#define print_res(out, fmt, ...) sprintf_s(out, ANS_SIZE, fmt, __VA_ARGS__)

#include <cstdint>
using i8  =   int8_t;
using u8  =  uint8_t;
using i16 =  int16_t;
using u16 = uint16_t;
using i32 =  int32_t;
using u32 = uint32_t;
using i64 =  int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

int solve(char p1[ANS_SIZE], char p2[ANS_SIZE]);
