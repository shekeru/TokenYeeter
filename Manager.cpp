#include "Injection.h"
#include <thread>
#include <chrono>

BOOL HackManager::AttachProcess(string procName)
{
	HANDLE hSnapshot;
	// Open Toolhelp Snapshot
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		cerr << "[AttachProcess] Snapshot failed"
			<< endl; goto bad_attach;
	} // Find Matching Process
	if (!Process32First(hSnapshot, &pEntry)) {
		cout << "[AttachProcess] Bad first process"
			<< endl; goto bad_attach;
	} do {
		if (!strcmp(procName.c_str(), pEntry.szExeFile)) {
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pEntry.th32ProcessID);
			CloseHandle(hSnapshot); return TRUE;
		}
	} while (Process32Next(hSnapshot, &pEntry));
bad_attach:
	CloseHandle(hSnapshot);
	return FALSE;
}

string HackManager::GetLocation(const UCHAR* Vec, size_t Len) {
	for (auto addr = 0ul; addr < 0x10000000ULL;) {
		MEMORY_BASIC_INFORMATION mbi; zero_struct(mbi);
		auto bytes = VirtualQueryEx(hProcess, (LPCVOID)addr, &mbi, sizeof(mbi));
		if (!bytes) {
			addr += 4096; continue;
		};
		if ((mbi.AllocationProtect & PAGE_READWRITE) == PAGE_READWRITE
			&& (mbi.AllocationProtect & PAGE_NOACCESS) != PAGE_NOACCESS
			&& (mbi.Protect & PAGE_GUARD) != PAGE_GUARD) {
			auto region = (UCHAR*)malloc(mbi.RegionSize);
			ReadProcessMemory(hProcess, mbi.BaseAddress, region, mbi.RegionSize, 0);
			for (auto i = 0; i < mbi.RegionSize - Len; i++) {
				bool found = true; auto j = 0ul;
				for (; j < Len; j++) {
					if (region[i + j] != Vec[j]) {
						found = false; break;
					}
				} if (found) {
					string copystr = ((CHAR*)region + i + j);
					free(region); return copystr;
				}
			}; free(region);
		}
		addr += mbi.RegionSize;
	}; printf("SCAN FAILURE: %s\n", Vec); return "";
};

HackManager::HackManager()
{
	pEntry.dwSize = sizeof(PROCESSENTRY32);
	using namespace std::chrono_literals;
	// Get Active Discord Process
	while (!AttachProcess("Discord.exe"))
		this_thread::sleep_for(1s);
}

HackManager::~HackManager()
{
	CloseHandle(hProcess);
}