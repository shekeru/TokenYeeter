#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <tchar.h>
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
	// memory scanning
	string GetLocation(const UCHAR* Vec, size_t Len ,int start = 0) {
		for (auto addr = 0ul; addr < 0x10000000ULL;) {
			MEMORY_BASIC_INFORMATION mbi; zero_struct(mbi);
			auto bytes = VirtualQueryEx(hProcess, (LPCVOID) addr, &mbi, sizeof(mbi));
			if (!bytes) {
				addr += 4096; continue;
			};
			if ((mbi.AllocationProtect & PAGE_READWRITE) == PAGE_READWRITE
					&& (mbi.AllocationProtect & PAGE_NOACCESS) != PAGE_NOACCESS
					&& (mbi.Protect & PAGE_GUARD) != PAGE_GUARD) {
				auto region = (UCHAR*) malloc(mbi.RegionSize);
				ReadProcessMemory(hProcess, mbi.BaseAddress, region, mbi.RegionSize, 0);
				for (auto i = 0; i < mbi.RegionSize - Len; i++) {
					bool found = true; auto j = 0ul;
					for (; j < Len; j++) {
						if (region[i + j] != Vec[j]) {
							found = false; break;
						}
					} if (found) {
						string copystr = ((CHAR*)region + i + j + start);
						free(region); return copystr;
					}
				}; free(region);
			}
			addr += mbi.RegionSize;
		}; printf("SCAN FAILURE: %s\n", Vec); return "";
	};
private:
	BOOL AttachProcess(string procName);
	HANDLE hProcess = INVALID_HANDLE_VALUE;
};