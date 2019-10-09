#include  "./Console.h"





void ConsoleOut::SetColor(unsigned int color)
{
	SetConsoleTextAttribute(hHandle, (WORD)((0 << 4) | color));

	return;
}

void ConsoleOut::SetRedColor()
{
	SetColor(COLOR_RED);

	return;
}

void ConsoleOut::SetGreenColor()
{
	SetColor(COLOR_GREEN);

	return;
}

void ConsoleOut::SetWhiteColor()
{
	SetColor(COLOR_WHITE);

	return;
}

ConsoleOut::ConsoleOut()
{
	hHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hHandle == 0)
		throw(112);

	return;

}

ConsoleOut::~ConsoleOut()
{
	
	return;
}

void ConsoleOut::error(const char *message)
{
	std::cout << "[";
	SetRedColor();
	std::cout << "ERROR";

	SetWhiteColor();
	std::cout << "] " << message << std::endl;

	return;
}

void ConsoleOut::error(const char *message, int num)
{
	std::cout << "[";
	SetRedColor();
	std::cout << "ERROR";

	SetWhiteColor();
	std::cout << "] 0x" << std::dec <<
	num << std::dec << " -> " <<
	message << std::endl;

	return;
}

void ConsoleOut::info(const char *message, bool newLine, bool prefix)
{
	if (prefix)
	{
		std::cout << "[";
		SetGreenColor();
		std::cout << "INFO";

		SetWhiteColor();
		std::cout << "] ";
	}

	std::cout << message;

	if (newLine)
		std::cout << std::endl;

	return;
}

void ConsoleOut::newline()
{
	std::cout << std::endl;

	return;
}
