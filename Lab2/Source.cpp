#include <iostream>
#include <vector>
#include <Windows.h>
using namespace std;

struct min_max_info
{
	vector<int>* arr;
	int min;
	int max;
};

struct average_info
{
	vector<int>* arr;
	double average;
};

using replaceFunc = void (*)(vector<int>&, int, int, int);

DWORD WINAPI min_max_thread(LPVOID lpParam)
{
	min_max_info* info = static_cast<min_max_info*>(lpParam);

	int min = info->arr->at(0);
	int max = info->arr->at(0);

	for (size_t i = 1; i < info->arr->size(); i++)
	{
		if (info->arr->at(i) < min)
			min = info->arr->at(i);
		Sleep(7);

		if (info->arr->at(i) > max)
			max = info->arr->at(i);
		Sleep(7);
	}

	info->min = min;
	info->max = max;

	cout << "min и max: min = " << min << ", max = " << max << endl;

	return 0;
}

DWORD WINAPI average_thread(LPVOID lpParam)
{
	average_info* info = static_cast<average_info*>(lpParam);

	int sum{};

	for (size_t i = 0; i < info->arr->size(); i++)
	{
		sum += info->arr->at(i);
		Sleep(12);
	}

	double avg = (double)sum / info->arr->size();
	info->average = round(avg);

	cout << "среднее: avg = " << avg << endl;

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	int n;
	cout << "Введите размер массива: ";
	cin >> n;
	if (n <= 0)
	{
		cout << "Неправильный размер";
		return 0;
	}

	vector<int> arr(n);
	cout << "Введите элементы массива: ";
	for (auto& i : arr)
	{
		cin >> i;
	}

	cout << "Массив до: ";
	for (auto& i : arr)
	{
		cout << i << " ";
	}
	cout << endl;

	min_max_info mmInfo;
	mmInfo.arr = &arr;

	average_info avgInfo;
	avgInfo.arr = &arr;

	HANDLE hMinMax = CreateThread(NULL, 0, min_max_thread, &mmInfo, 0, NULL);
	HANDLE hAverage = CreateThread(NULL, 0, average_thread, &avgInfo, 0, NULL);

	WaitForSingleObject(hMinMax, INFINITE);
	WaitForSingleObject(hAverage, INFINITE);

	CloseHandle(hMinMax);
	CloseHandle(hAverage);

	HMODULE hLib = LoadLibraryA("Replace.dll");
	if (hLib == NULL)
	{
		cout << "Библиотека не загрузилась";
		return 1;
	}

	replaceFunc replace_func = (replaceFunc)GetProcAddress(hLib, "replace_min_max");
	replace_func(arr, mmInfo.min, mmInfo.max, (int)avgInfo.average);

	FreeLibrary(hLib);

	cout << "Массив после: ";
	for (auto it : arr) 
		cout << it << " ";

	cout << endl;
}