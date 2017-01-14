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
#include <curl/curl.h> /// pkg "libcurl-devel" (Fedora Linux 24)
#include "logger.h"
#include "shapeshift-api.h"
#include "shapeshift-api-debug.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "http.h"

#ifdef __linux__
#include <unistd.h>
#endif // __linux__

using namespace std;

int main(int argc, const char* argv[])
{
	test_all(true);

	int breakpointHere = 0;

    return breakpointHere;
}
