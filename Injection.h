#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
using namespace std;

template <typename T>
void zero_struct(T& mem) {
	memset(&mem, 0, sizeof(mem));
}

class HackManager
{
public:
	PROCESSENTRY32 pEntry;
	HackManager(); ~HackManager();
	string GetLocation(const UCHAR* Vec, size_t Len);
private:
	BOOL AttachProcess(string procName);
	HANDLE hProcess = INVALID_HANDLE_VALUE;
};