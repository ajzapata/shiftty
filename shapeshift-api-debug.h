#ifndef SHAPESHIFT_API_DEBUG_INCLUDED
#define SHAPESHIFT_API_DEBUG_INCLUDED

#include "shapeshift-api.h"
#include <string>
#include <cstdint>

using std::string;

/// Test-case return values
const int TEST_EXCEPT					= -2;
const int TEST_ERR						= -1;
const int TEST_OK						= 0;
const int TEST_API_ERR					= 1;
const int TEST_API_UNEXPECTED_VALUE		= 2;

/// Test-case default values (for batch tests)
/// For now, the checks only validate API responses, rather than complete and
/// successful execution (e.g. transaction check won't verify that coins were
/// correctly sent and received)
const string DEFAULT_COIN_PAIR = "btc_xmr";

/// Test cases: Check for expected output
int test_rate(string coin_pair, bool verbose = false);
int test_depositLimit(string coin_pair, bool verbose = false);
int test_marketInfo(string coin_pair, bool verbose = false);
#if 0
int test_recentTransactions(uint8_t amount = 5, bool verbose = false);
int test_transactionStatus(string address_in, bool verbose = false);
int test_timeRemaining(string deposit_address, bool verbose = false);
#endif
#if 0
int test_listCoins(bool verbose = false);
int test_listTransactions_private(string api_key, string address_in = "",
	bool verbose = false);
int test_validateAddress(string address, bool verbose = false);
int test_createTransaction_quick(string address_out, string coin_pair,
	string return_address = "", string api_public_key = "",
	bool verbose = false);
int test_requestEmailReceipt(string email_address, string tx_id,
	bool verbose = false);
int test_createTransaction(string address_out, double amount, string coin_pair,
	bool getQuoteOnly = false, string return_address = "",
	string api_public_key = "", bool verbose = false);
int test_cancelTransaction_obj api_cancelTransaction(string address_in,
	bool verbose = false);
#endif // 0

/// Test-case batches
/// These are relatively automatic and use preset values defined above
int test_all(bool verbose = false);

#endif // SHAPESHIFT_API_DEBUG_INCLUDED
