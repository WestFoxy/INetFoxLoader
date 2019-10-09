#include "./Injector.h"
#include "xorstr.h"

using namespace std;

DllJct::DllJct()
{
	ProcessHandle = 0;
	path = new char[MAX_PATH];

	return;
}

DllJct::~DllJct()
{
	delete[] path;

	return;
}

char *DllJct::LocalInject()
{
	char DllName[MAX_PATH], tempdat[5];
	BYTE pressedKey;

	EnumerateAllFiles();

	if (foundDll.size() == 0)
		throw(runtime_error(xorstr_("DLL NOT FOUND")));

	if (foundDll.size() < 9)
	{
		GetCurrentDirectoryA(MAX_PATH, path);

		info(path);
		info("Available dlls: ");

		for (DWORD count(1); count < foundDll.size() + 1; count++)
		{
			snprintf(tempdat, 5, "%d", count);

			info(tempdat, false, false);
			info(". ", false, false);
			info(foundDll[count - 1].c_str(), true, false);

		}

		newline();

		info("Enter number: ");

	}
	else
		throw(runtime_error("more than 9 d11 in folder..."));


	while (true)
	{
		for (BYTE key(0x31); key < (0x31 + foundDll.size()); key += 0x1)
		{

			if (GetAsyncKeyState((int)key) & 1)
			{

				pressedKey = key;

				goto end;

			}

		}

		Sleep(10);
	}

end:

	switch (pressedKey)
	{
	case 0x31:
		strcpy(DllName, foundDll[0].c_str());

		break;
	case 0x32:
		strcpy(DllName, foundDll[1].c_str());

		break;
	case 0x33:
		strcpy(DllName, foundDll[2].c_str());

		break;
	case 0x34:
		strcpy(DllName, foundDll[3].c_str());

		break;
	case 0x35:
		strcpy(DllName, foundDll[4].c_str());

		break;
	case 0x36:
		strcpy(DllName, foundDll[5].c_str());

		break;
	case 0x37:
		strcpy(DllName, foundDll[6].c_str());

		break;
	case 0x38:
		strcpy(DllName, foundDll[7].c_str());

		break;
	case 0x39:
		strcpy(DllName, foundDll[8].c_str());

		break;

	default:
		throw(runtime_error("INSUFFICENT KEY"));

	}

	info("Loading ", false);
	info(DllName, false, false);
	info(" ...", true, false);

	GetFullPathNameA(DllName, MAX_PATH, path, NULL);
	IfExists(DllName);

	char ProcessNameP[MAX_PATH];

	info(xorstr_("Process name: "), false);
	std::cin >> ProcessNameP;

	RemoteThreadInject(ProcessNameP);

	

	return DllName;
}

void DllJct::DoRoutine(const char * ProcessName)
{
	char DllName[MAX_PATH];

	if (GetAsyncKeyState(VK_INSERT))
		LocalInject();
	else
		DownloadFileAndInject();
	


	return;
}

void DllJct::RemoteThreadInject(const char *ProcessNameP)
{
	pId = GetPidByName(ProcessNameP);

	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pId);

	if (ProcessHandle <= NULL) {
		throw(runtime_error(xorstr_("REMOTE HANDLE ERROR")));

		return;
	}

	info(path);

	LPVOID LoadLibraryAdd = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	LPVOID rString = (LPVOID)VirtualAllocEx(ProcessHandle, NULL, strlen(path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (rString == NULL) {
		CloseHandle(ProcessHandle);

		throw(runtime_error(xorstr_("VirtualAllocEx ERROR")));
		return ;
	}
	if (!(WriteProcessMemory(ProcessHandle, (LPVOID)rString, path, strlen(path), NULL))) {
		CloseHandle(ProcessHandle);

		throw(runtime_error(xorstr_("WriteProcessMemory ERROR")));
		return;
	}
	HANDLE hThread = CreateRemoteThread(ProcessHandle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryAdd, (LPVOID)rString, NULL, NULL);
	if (hThread == NULL) {
		CloseHandle(ProcessHandle);

		throw(runtime_error(xorstr_("CreateRemoteThread ERROR")));
		return;
	}

	CloseHandle(ProcessHandle);
}

void DllJct::DownloadFileAndInject()
{
	info(xorstr_("Downloading file"));
}

DWORD DllJct::GetPidByName(const char *name)
{

	PROCESSENTRY32 ProcessEntry;
	HANDLE hHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hHandle == INVALID_HANDLE_VALUE) {
		throw(10);

		return NULL;
	}
	ProcessEntry.dwSize = sizeof(ProcessEntry);
	bool loop = Process32First(hHandle, &ProcessEntry);
	while (loop) {
		if (!(_strcmpi(ProcessEntry.szExeFile, name))) {
			CloseHandle(hHandle);
			return ProcessEntry.th32ProcessID;
		}
		loop = Process32Next(hHandle, &ProcessEntry);
	}
	CloseHandle(hHandle);
	
	throw(11);

	return NULL;
}
