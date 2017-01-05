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
#include "shapeshift-api.h"
#include "shapeshift-api-debug.h"

/* OS-specific Libraries */

#ifndef WIN32
#include <unistd.h>
#endif

/* CURL Libraries (from package "libcurl-devel" (Fedora Linux 24) */

#include <curl/curl.h>

using namespace std;

int main(int argc, const char* argv[])
{
	test_all(true);

	int breakpointHere = 0;
	log("Hello world!");

    return breakpointHere;
}
