
#include "SDK.h"
#include "Injector.h"
#include "Internet.h"
#include <vcruntime_exception.h>

#include "xorstr.h"

using namespace std;


ConsoleOut *foxOut;
DllJct *foxJct;

char *foundings;
string bData;


void main()
{
	foxOut = new ConsoleOut;
	foxJct = new DllJct;

	foundings = 0;

	try
	{
		foxJct->DoRoutine("fox");
	}
	catch (const runtime_error& error)
	{
		Beep(400, 150);
		Beep(400, 150);

		foxOut->error(error.what());
	}

	delete foxOut;
	delete foxJct;

	return;

}
