#include "shapeshift-api-debug.h"
#include <iostream>
#include <string>

using namespace std;

/// Test cases

int test_rate(string coin_pair, bool verbose)
{
	api_rate_obj obj = api_rate(coin_pair);

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_rate #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "coin_pair: " << coin_pair << endl;
		cerr << "Function output:" << endl;
		cerr << "obj.coin_pair = " << obj.coin_pair << endl;
		cerr << "obj.rate = " << obj.rate << endl;
		cerr << "obj.error = " << obj.error << endl;
		cerr << "##### END TESTING api_rate #####" << endl;
	}

	return TEST_OK;
}
