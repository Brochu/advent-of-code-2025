#pragma once

#include <vector>

struct strview {
    const char* ptr;
    size_t len;
};

inline strview sv(const char *s) {
    return { s, strlen(s) };
}

strview sv_find(strview haystack, strview needle);
strview sv_find(strview haystack, const char *needle);

std::vector<strview> sv_split(strview str, strview delim);
std::vector<strview> sv_split(strview str, const char *delim);

bool sv_split_once(strview str, strview delim, strview* first, strview* second);
bool sv_split_once(strview str, const char *delim, strview* first, strview* second);
