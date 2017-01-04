/******************************************************************************
shiftty (ShifTTY) - Shapeshift Cryptocurrency Exchange Terminal

ShifTTY is a command-line interface to the cryptocurrency exchange Shapeshift
using its API.

******************************************************************************/

/* C++ Standard Library */

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cassert>
#include <vector>
#include "http.h"
#include "json.h"
#include "logger.h"
#include "shapeshift-api.h"

/* OS-specific Libraries */

#ifndef WIN32
#include <unistd.h>
#endif

/* CURL Libraries (from package "libcurl-devel" (Fedora Linux 24) */

#include <curl/curl.h>

using namespace std;

int main(int argc, const char* argv[])
{
	api_rate_obj obj = api_rate("btc_xmr");

	if (obj.error == "") {
		cout << "coin_pair: " << obj.coin_pair << endl;
		cout << "rate: " << obj.rate << endl;
	} else
		cout << "error: " << obj.error << endl;

	int breakpointHere = 0;
	log("Hello world!");

    return breakpointHere;
}
