#pragma once
#include "SDK.h"

class FileManager : public ConsoleOut
{
	
public:
	char path[MAX_PATH];

	HANDLE hFindFile;
	std::ifstream tempfile;

	std::vector <std::string>foundDll;
	std::string foundHackDll;
	std::string foundChangerDll;

	FileManager();
	~FileManager();

	bool Check(const BYTE* pData, const BYTE* bMask, const char* szMask);
	DWORD FindPattern(BYTE *bMask, char* szMask, DWORD dwOffset, DWORD dwLen, DWORD dwAddress);

	void IfExists(const char *filename);
	void EnumerateAllFiles();
	void CheckSignature();

	
};