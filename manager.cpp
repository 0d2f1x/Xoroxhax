#include "manager.h"

manager::manager() {
	handle = NULL;
}

manager::~manager() {
	CloseHandle(handle);
}

unsigned int manager::getProcess(const char* proc) {
	HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	unsigned int process;
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	do {
		if (!strcmp(pEntry.szExeFile, proc)) {
			process = pEntry.th32ProcessID;
			CloseHandle(hProcessId);
			handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
		}

	} while (Process32Next(hProcessId, &pEntry));
	return process;
}

uintptr_t manager::getModule(unsigned int procId, const char* modName) {
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	do {
		if (!strcmp(mEntry.szModule, modName)) {
			CloseHandle(hModule);
			return (unsigned int)mEntry.hModule;
		}
	} while (Module32Next(hModule, &mEntry));
	return 0;
}

unsigned int manager::getAddress(unsigned int addr, std::vector<unsigned int> vect) {
	for (unsigned int i = 0; i < vect.size(); i++) {
		ReadProcessMemory(handle, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += vect[i];
	}
	return addr;
}