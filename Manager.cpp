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