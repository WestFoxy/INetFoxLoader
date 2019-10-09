#include "FileManager.h"

using namespace std;


FileManager::FileManager() : ConsoleOut()
{

	return;
}

FileManager::~FileManager()
{

	return;
}

void FileManager::IfExists(const char * filename)
{
	tempfile.open(filename, std::ios::in);

	if (!tempfile.is_open())
		throw(runtime_error("IfExists() failed"));

	tempfile.close();

	return;

}

void FileManager::EnumerateAllFiles()
{
	std::string tempdat;

	GetCurrentDirectoryA(MAX_PATH, path);
	tempdat += path;
	tempdat += "\\*";


	WIN32_FIND_DATA fNdData;
	hFindFile = FindFirstFileA(tempdat.c_str(), &fNdData);

	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fNdData.dwFileAttributes == 32 && strstr(fNdData.cFileName, ".dll"))
			{
				//info(fNdData.cFileName);
				tempdat.clear();

				tempdat += fNdData.cFileName;

				foundDll.push_back(tempdat);
			}
			

		} while (FindNextFileA(hFindFile, &fNdData));

		FindClose(hFindFile);
	}

	return;
}

struct HexCharStruct
{
	unsigned char c;
	HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
	return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
	return HexCharStruct(_c);
}

bool FileManager::Check(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask != '?' && *pData != *bMask)
			return false;

	return (*szMask) == NULL;
}

DWORD FileManager::FindPattern(BYTE *bMask, char* szMask, DWORD dwOffset, DWORD dwLen, DWORD dwAddress)
{

	__try
	{
		for (DWORD i = 0; i < dwLen; i++)
			if (Check((BYTE*)(dwAddress + i), bMask, szMask))
				return (DWORD)(dwAddress + i + dwOffset);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		MessageBox(NULL, "Find Pattern Error", "Error", MB_OK);
	}

	return 0x00;
}

void FileManager::CheckSignature()
{
	

}
