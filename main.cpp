/******************************************************************************
shiftty (ShifTTY) - Shapeshift Cryptocurrency Exchange Terminal

ShifTTY is a command-line interface to the cryptocurrency exchange Shapeshift
using its API.

******************************************************************************/

#include <cstdlib>
#include <cinttypes>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include "logger.h"
//#include "shapeshift-api.h"
//#include "shapeshift-api-debug.h"

/* OS-specific Libraries */

#ifdef __linux__
#include <unistd.h>
#include <curl/curl.h> /// pkg "libcurl-devel" (Fedora Linux 24)
#else /// Microsoft Windows (x64)
#include <Windows.h>
#endif

using namespace std;

int main(int argc, const char* argv[])
{
	slog("Hello world!", MessageType::info);

	int breakpointHere = 0;

    return breakpointHere;
}
