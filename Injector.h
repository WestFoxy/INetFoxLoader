
#pragma once
#include "SDK.h"
#include "FileManager.h"



class DllJct : public FileManager
{
public:
	char *path;

	HANDLE ProcessHandle;
	DWORD pId;

	DWORD GetPidByName(const char *);

	char* LocalInject();

	DllJct();
	~DllJct();

	void DoRoutine(const char *);
	void RemoteThreadInject(const char*);
	void DownloadFileAndInject();
};