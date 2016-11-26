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
	vector<api_recentTransactions_obj> obj = api_recentTransactions(-1);

	if (obj[0].error != "") {
		/// Error condition
		cout << "ERROR: " << obj[0].error << endl;
		return 1;
	}
	else {
		cout << "VECTOR SIZE: " << obj.size() << endl;
		cout << endl << "FIRST ITEM IN VECTOR:" << endl;
		cout << "   curIn: " << obj[0].coin_in << endl;
		cout << "   curOut: " << obj[0].coin_out << endl;
		cout << "   timestamp: " << obj[0].timestamp_sec << endl;
		cout << "   amount: " << obj[0].amount << endl;
	}

    return 0;
}
