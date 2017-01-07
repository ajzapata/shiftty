#include "shapeshift-api-debug.h"
#include <iostream>
#include <string>
#include <vector>

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
			/// Technically string objects like obj.coin_pair aren't undefined
			/// due to having a constructor, but it's preferable to not try
			/// accessing them as they're useless on their own anyways...
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

int test_marketInfo(string coin_pair, bool verbose)
{
	/// Note that the API function returns a vector of objects
    vector<api_marketInfo_obj> obj = api_marketInfo(coin_pair);

	/// Check for API errors; the error is reported in the first object
	int i_retval = obj[0].error != "" ? TEST_API_ERR : TEST_OK;
	string s_retval = obj[0].error != "" ? "TEST_API_ERR" : "TEST_OK";

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_marketInfo #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "coin_pair = " << coin_pair << endl;
		cerr << "Function output:" << endl;
		cerr << "retval = " << s_retval << endl;
		if (obj[0].error != "") /// API error
		{
			cerr << "obj[0].coin_pair = (UNDEFINED)" << endl;
			cerr << "obj[0].rate = (UNDEFINED)" << endl;
			cerr << "obj[0].limit_min = (UNDEFINED)" << endl;
			cerr << "obj[0].limit_qmax = (UNDEFINED)" << endl;
			cerr << "obj[0].limit_max = (UNDEFINED)" << endl;
			cerr << "obj[0].minerfee = (UNDEFINED)" << endl;
			cerr << "obj[0].error = " << obj[0].error << endl;
		}
		else
		{
			string objstr;

			/// We iterate through all the coin pairs
			for (size_t i = 0; i < obj.size(); i++)
			{
				objstr = "obj[" + to_string(i) + "]";
				cerr << objstr + ".coin_pair = " << obj[i].coin_pair << endl;
				cerr << objstr + ".rate = " << obj[i].rate << endl;
				cerr << objstr + ".limit_min = " << obj[i].limit_min << endl;
				cerr << objstr + ".limit_qmax = " << obj[i].limit_qmax << endl;
				cerr << objstr + ".limit_max = " << obj[i].limit_max << endl;
				cerr << objstr + ".minerfee = " << obj[i].minerfee << endl;
				cerr << objstr + ".error = " << obj[i].error << endl;
			}
		}
		cerr << "##### END TESTING api_marketInfo #####" << endl;
	}

	return i_retval;
}

int test_recentTransactions(uint8_t amount, bool verbose)
{
	/// Note that the API function returns a vector of objects
    vector<api_recentTransactions_obj> obj = api_recentTransactions(amount);

	/// Check for API errors; the error is reported in the first object
	int i_retval = obj[0].error != "" ? TEST_API_ERR : TEST_OK;
	string s_retval = obj[0].error != "" ? "TEST_API_ERR" : "TEST_OK";

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_recentTransactions #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "amount = " << amount << endl;
		cerr << "Function output:" << endl;
		cerr << "retval = " << s_retval << endl;
		if (obj[0].error != "") /// API error
		{
			cerr << "obj[0].coin_in = (UNDEFINED)" << endl;
			cerr << "obj[0].coin_out = (UNDEFINED)" << endl;
			cerr << "obj[0].amount = (UNDEFINED)" << endl;
			cerr << "obj[0].timestamp_sec = (UNDEFINED)" << endl;
			cerr << "obj[0].error = " << obj[0].error << endl;
		}
		else
		{
			string objstr;

			/// We iterate through all the transactions
			for (size_t i = 0; i < obj.size(); i++)
			{
				objstr = "obj[" + to_string(i) + "]";
				cerr << objstr + ".coin_in = " << obj[i].coin_in << endl;
				cerr << objstr + ".coin_out = " << obj[i].coin_out << endl;
				cerr << objstr + ".amount = " << obj[i].amount << endl;
				cerr << objstr + ".timestamp_sec = "
					<< obj[i].timestamp_sec << endl;
				cerr << objstr + ".error = " << obj[i].error << endl;
			}
		}
		cerr << "##### END TESTING api_recentTransactions #####" << endl;
	}

	return i_retval;
}

/// Testing incomplete: For best results, this test requires a live transaction
int test_transactionStatus(string address_in, bool verbose)
{
    api_transactionStatus_obj obj = api_transactionStatus(address_in);

	/// Check for API errors
	int i_retval = obj.error != "" ? TEST_API_ERR : TEST_OK;
	string s_retval = obj.error != "" ? "TEST_API_ERR" : "TEST_OK";

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_transactionStatus #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "address_in = " << address_in << endl;
		cerr << "Function output:" << endl;
		cerr << "retval = " << s_retval << endl;
		if (obj.error != "") /// API error
		{
			/// The status and address_in strings are frequently set when an
			/// error occurs (e.g. invalid address or failed tx). Since C++
			/// strings are default-constructed as empty-strings, it isn't
			/// particularly hazardous to dereference them in this case; this
			/// is considered temporary until a more rigorous testing system
			/// can be established.
			cerr << "obj.status = " << obj.status << endl;
			cerr << "obj.address_in = " << obj.address_in << endl;
			cerr << "obj.address_out = (UNDEFINED)" << endl;
			cerr << "obj.coin_in = (UNDEFINED)" << endl;
			cerr << "obj.amount_in = (UNDEFINED)" << endl;
			cerr << "obj.coin_out = (UNDEFINED)" << endl;
			cerr << "obj.amount_out = (UNDEFINED)" << endl;
			cerr << "obj.tx_id = (UNDEFINED)" << endl;
		}
		else if (obj.status == "no_deposits")
		{
			cerr << "obj.status = " << obj.status << endl;
			cerr << "obj.address_in = " << obj.address_in << endl;
			cerr << "obj.address_out = (UNDEFINED)" << endl;
			cerr << "obj.coin_in = (UNDEFINED)" << endl;
			cerr << "obj.amount_in = (UNDEFINED)" << endl;
			cerr << "obj.coin_out = (UNDEFINED)" << endl;
			cerr << "obj.amount_out = (UNDEFINED)" << endl;
			cerr << "obj.tx_id = (UNDEFINED)" << endl;
		}
		else if (obj.status == "received")
		{
			cerr << "obj.status = " << obj.status << endl;
			cerr << "obj.address_in = " << obj.address_in << endl;
			cerr << "obj.address_out = (UNDEFINED)" << endl;
			cerr << "obj.coin_in = (UNDEFINED)" << endl;
			cerr << "obj.amount_in = (UNDEFINED)" << endl;
			cerr << "obj.coin_out = (UNDEFINED)" << endl;
			cerr << "obj.amount_out = (UNDEFINED)" << endl;
			cerr << "obj.tx_id = (UNDEFINED)" << endl;
		}
		else if (obj.status == "complete")
		{
			cerr << "obj.status = " << obj.status << endl;
			cerr << "obj.address_in = " << obj.address_in << endl;
			cerr << "obj.address_out = " << obj.address_out << endl;
			cerr << "obj.coin_in = " << obj.coin_in << endl;
			cerr << "obj.amount_in = " << obj.amount_in << endl;
			cerr << "obj.coin_out = " << obj.coin_out << endl;
			cerr << "obj.amount_out = " << obj.amount_out << endl;
			cerr << "obj.tx_id = " << obj.tx_id << endl;
		}
		cerr << "obj.error = " << obj.error << endl;
		cerr << "##### END TESTING api_transactionStatus #####" << endl;
	}

	return i_retval;
}

/// Testing incomplete: For best results, this test requires a live transaction
int test_timeRemaining(string deposit_address, bool verbose)
{
    api_timeRemaining_obj obj = api_timeRemaining(deposit_address);

	/// Check for API errors
	int i_retval = obj.error != "" ? TEST_API_ERR : TEST_OK;
	string s_retval = obj.error != "" ? "TEST_API_ERR" : "TEST_OK";

	/// Print API call output
	if (verbose)
	{
		cerr << "##### TESTING api_timeRemaining #####" << endl;
		cerr << "Function arguments:" << endl;
		cerr << "deposit_address = " << deposit_address << endl;
		cerr << "Function output:" << endl;
		cerr << "retval = " << s_retval << endl;
		if (obj.error != "") /// API error
		{
			cerr << "obj.status = (UNDEFINED)" << endl;
			cerr << "obj.seconds_remaining = (UNDEFINED)" << endl;
		}
		else
		{
			cerr << "obj.status = " << obj.status << endl;
			cerr << "obj.seconds_remaining = " << obj.seconds_remaining
				<< endl;
		}
		cerr << "obj.error = " << obj.error << endl;
		cerr << "##### END TESTING api_timeRemaining #####" << endl;
	}

	return i_retval;
}

/// Test-case batches

int test_all(bool verbose)
{
	cerr << "##### BEGIN BATCH TEST ALL #####" << endl;

	int retval_rate = test_rate(DEFAULT_COIN_PAIR, verbose);
	int retval_depositLimit = test_depositLimit(DEFAULT_COIN_PAIR, verbose);
	int retval_marketInfo = test_marketInfo(DEFAULT_COIN_PAIR, verbose);
	int retval_recentTransactions = test_recentTransactions(DEFAULT_AMOUNT,
		verbose);
	int retval_transactionStatus = test_transactionStatus(DEFAULT_ADDRESS_IN,
		verbose);
	int retval_timeRemaining = test_timeRemaining(DEFAULT_DEPOSIT_ADDRESS,
		verbose);

	bool pass = !(retval_rate || retval_depositLimit || retval_marketInfo ||
		retval_recentTransactions || !(retval_transactionStatus) ||
		!(retval_timeRemaining)); // negated values need "live" (better) data
	string s_pass = pass ? "PASS" : "FAIL";

	cerr << "RESULT: " << s_pass << endl;
	cerr << "(0: TEST_OK | 1: TEST_API_ERR | 2: TEST_API_UNEXPECTED_VALUE)"
		<< endl << endl;

	/// List tests; mark those that failed (list is incomplete ^)
	cerr << "[" << retval_rate << "] test_rate" << endl;
	cerr << "[" << retval_depositLimit << "] test_depositLimit" << endl;
	cerr << "[" << retval_marketInfo << "] test_marketInfo" << endl;
	cerr << "[" << retval_recentTransactions << "] test_recentTransactions"
		<< endl;
	cerr << "[" << retval_transactionStatus << "] test_transactionStatus*"
		<< endl;
	cerr << "[" << retval_timeRemaining << "] test_timeRemaining*" << endl;
	cerr << "*Non-zero number indicates passed test." << endl;

	cerr << "##### END BATCH TEST ALL #####" << endl;

	/// '0' = pass (as a return value)
	return !pass;
}
