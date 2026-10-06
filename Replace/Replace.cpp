#include "pch.h"
#include "Replace.h"
using namespace std;

void replace_min_max(vector<int>& arr, int min, int max, int avg)
{
	for (size_t i = 0; i < arr.size(); i++)
	{
		if (arr[i] == min || arr[i] == max)
			arr[i] = avg;
	}
}

