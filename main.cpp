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
#include "http.h"
#include "json.h"
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
	api_depositLimit_obj obj = api_depositLimit("btc_ltc");

	if (obj.error != "") {
		/// Error condition
		cout << "ERROR: " << obj.error << endl;
		return 1;
	}
	else {
		cout << "coin_pair: " << obj.coin_pair << endl;
		cout << "limit: " << obj.limit_max << endl;
		cout << "min: " << obj.limit_min << endl;
	}

    return 0;
}
