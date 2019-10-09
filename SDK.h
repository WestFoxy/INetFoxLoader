//#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define DLL_NAME "FOX.dll"

#include <winsock2.h>
//#include <ws2tcpip.h>

#include <Windows.h>

#include <WinInet.h>
#include <TlHelp32.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <math.h>
#include <fstream>


#pragma comment(lib,"wininet")
#pragma comment(lib, "Ws2_32.lib")

#define repeat(x) for(unsigned int count(0); count < x; count++)
#define pause std::system("pause")

#include "Console.h"


