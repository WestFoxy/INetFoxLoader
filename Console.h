#pragma once
#include "SDK.h"

enum
{
	COLOR_GREEN = 10,
	COLOR_RED = 12,
	COLOR_WHITE = 15
};

class FileManager;
class ConsoleOut
{
public:
	HANDLE hHandle;

	virtual void SetColor(unsigned int color);
	virtual void SetRedColor();
	virtual void SetGreenColor();
	virtual void SetWhiteColor();

	ConsoleOut();
	virtual ~ConsoleOut();

	virtual void error(const char *);
	virtual void error(const char *, int);
	virtual void info(const char *, bool newLine = true, bool prefix = true);
	virtual void newline();
};