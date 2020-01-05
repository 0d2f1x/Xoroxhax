#pragma once
#include <Windows.h>
#include <vector>
#include <TlHelp32.h>
#include <string>

using namespace std;

class manager
{
public:
	unsigned int getProcess(const char*);
	uintptr_t getModule(unsigned int, const char*);
	unsigned int getAddress(unsigned int, std::vector<unsigned int>);
	HANDLE handle;

	manager();
	~manager();

	template <class type>
	type RPM(unsigned int addr) {
		type x;
		ReadProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template <class type>
	type WPM(unsigned int addr, type x) {
		WriteProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return 0;
	}

	string RS(unsigned int addr)
	{
		char string[256];
		for (int i = 0; string[i - 1] > 0; i++)
			string[i] = RPM<char>(addr + i);
		return string;
	}
};