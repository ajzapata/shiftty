#include "shapeshift-api-debug.h"
#include <iostream>
#include <string>

using namespace std;

/// Test cases

int test_rate(string coin_pair, bool verbose)
{
	api_rate_obj obj = api_rate(coin_pair);

	/// Check for API errors
	int i_retval = obj.error != "" ? TEST_API_ERR : TEST_OK;
	string s_retval = obj.error != "" ? "TEST_API_ERR" : "TEST_OK";

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_rate #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "coin_pair = " << coin_pair << endl;
		cerr << "Function output:" << endl;
		cerr << "retval = " << s_retval << endl;
		if (obj.error != "") /// API error
		{
			cerr << "obj.coin_pair = (UNDEFINED)" << endl;
			cerr << "obj.rate = (UNDEFINED)" << endl;
		}
		else
		{
			cerr << "obj.coin_pair = " << obj.coin_pair << endl;
			cerr << "obj.rate = " << obj.rate << endl;
		}
		cerr << "obj.error = " << obj.error << endl;
		cerr << "##### END TESTING api_rate #####" << endl;
	}

	return i_retval;
}

int test_depositLimit(string coin_pair, bool verbose)
{
    api_depositLimit_obj obj = api_depositLimit(coin_pair);

	/// Check for API errors
	int i_retval = obj.error != "" ? TEST_API_ERR : TEST_OK;
	string s_retval = obj.error != "" ? "TEST_API_ERR" : "TEST_OK";

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_depositLimit #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "coin_pair = " << coin_pair << endl;
		cerr << "Function output:" << endl;
		cerr << "retval = " << s_retval << endl;
		if (obj.error != "") /// API error
		{
			cerr << "obj.coin_pair = (UNDEFINED)" << endl;
			cerr << "obj.limit_max = (UNDEFINED)" << endl;
			cerr << "obj.limit_min = (UNDEFINED)" << endl;
		}
		else
		{
			cerr << "obj.coin_pair = " << obj.coin_pair << endl;
			cerr << "obj.limit_max = " << obj.limit_max << endl;
			cerr << "obj.limit_min = " << obj.limit_min << endl;
		}
		cerr << "obj.error = " << obj.error << endl;
		cerr << "##### END TESTING api_depositLimit #####" << endl;
	}

	return i_retval;
}
