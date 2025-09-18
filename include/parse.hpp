#pragma once

#include <vector>

struct strview {
    const char *ptr;
    size_t len;

    strview() : ptr(NULL), len(0) {}
    strview(const char *s) : ptr(s), len(strlen(s)) {}
    strview(const char *s, size_t n) : ptr(s), len(n) {}
};

strview sv_strstr(strview haystack, strview needle);
std::vector<strview> sv_split(strview str, strview delim);
bool sv_split_once(strview str, strview delim, strview* first, strview* second);
