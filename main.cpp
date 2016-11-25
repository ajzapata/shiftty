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
	vector<api_marketInfo_obj> obj = api_marketInfo("btc_ltc");

	if (obj[0].error != "") {
		/// Error condition
		cout << "ERROR: " << obj[0].error << endl;
		return 1;
	}
	else {
		cout << "VECTOR SIZE: " << obj.size() << endl;
		cout << endl << "FIRST ITEM IN VECTOR:" << endl;
		cout << "   coin_pair: " << obj[0].coin_pair << endl;
		cout << "   rate: " << obj[0].rate << endl;
		cout << "   minerFee: " << obj[0].minerfee << endl;
		cout << "   limit: " << obj[0].limit_qmax << endl;
		cout << "   minimum: " << obj[0].limit_min << endl;
		cout << "   maxLimit: " << obj[0].limit_max << endl;
	}

    return 0;
}
