#pragma once
#include <vector>

#ifdef REPLACE_EXPORTS
#define REPLACE_API __declspec(dllexport)
#else
#define REPLACE_API __declspec(dllimport)
#endif

extern "C" REPLACE_API void replace_min_max(std::vector<int>& arr, int min, int max, int avg);