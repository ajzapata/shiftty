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
#include "json/json.h"
#include "json/json-forwards.h"
#include "http.h"

/* OS-specific Libraries */

#ifdef __linux__
#include <unistd.h>
#include <curl/curl.h> /// pkg "libcurl-devel" (Fedora Linux 24)
#else /// Microsoft Windows (x64)
//#include <Windows.h>
//#include <WinSock2.h>
#endif

using namespace std;

int main(int argc, const char* argv[])
{
	//slog("Hello world!", MessageType::info);
	//string json_raw = "{\"lorem\":\"ipsum\"}";
	//Json::Reader json_reader;
	//Json::Value json_data;

	//assert(json_reader.parse(json_raw, json_data));
	//cout << json_data["lorem"].asString() << endl;

	string url = "www.example.com";//"https://shapeshift.io/rate/xmr_btc";
	cout << http_get(url) << endl;

	int breakpointHere = 0;

    return breakpointHere;
}
