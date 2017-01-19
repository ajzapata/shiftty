/******************************************************************************
shiftty (ShifTTY) - Shapeshift Cryptocurrency Exchange Terminal

ShifTTY is a command-line interface to the cryptocurrency exchange Shapeshift
using its API.

******************************************************************************/

#include "shiftty.h"
#include <cstdlib>
#include <cinttypes>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <curl/curl.h> /// pkg "libcurl-devel" (Fedora Linux 24)
#include "shapeshift-api.h"
#include "shapeshift-api-debug.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include "http.h"

#if !defined _WIN32 && !defined _WIN64
#include <unistd.h>
#endif // !defined _WIN32 && !defined _WIN64

using namespace std;

int main(int argc, const char* argv[])
{
    test_all(true);

    return 0;
}
