#ifndef SHAPESHIFT_API_INCLUDED
#define SHAPESHIFT_API_INCLUDED

#include <cstdlib>
#include <string>
#include <vector>
#include <cinttypes>

using std::string;
using std::vector;

/// COIN FORMAT: IN_OUT

/// API RETURN OBJECTS: ALWAYS CHECK FOR ERROR MESSAGE (i.e. not empty)
/// OTHERWISE, ACCESS OF OTHER VARS ARE UNDEFINED

/* Constants */

/// URLS
const string
URL_API_RATE = "https://shapeshift.io/rate/",
URL_API_DEPOSIT_LIMIT = "https://shapeshift.io/limit/",
URL_API_MARKET_INFO = "https://shapeshift.io/marketinfo/",
URL_API_RECENT_TRANSACTIONS = "https://shapeshift.io/recenttx/",
URL_API_TRANSACTION_STATUS = "https://shapeshift.io/txStat/",
URL_API_TIME_REMAINING = "https://shapeshift.io/timeremaining/",
URL_API_LIST_COINS = "https://shapeshift.io/getcoins",
URL_API_LIST_TRANSACTIONS_PRIVATE = "https://shapeshift.io/txbyapikey/",
URL_API_LIST_TRANSACTIONS_PRIVATE_ADDR = "https://shapeshift.io/txbyaddress/",
URL_API_VALIDATE_ADDRESS = "https://shapeshift.io/validateAddress/",
URL_API_CREATE_TRANSACTION_QUICK = "https://shapeshift.io/shift",
URL_API_REQUEST_EMAIL_RECEIPT = "https://shapeshift.io/mail",
URL_API_CREATE_TRANSACTION = "https://shapeshift.io/sendamount",
URL_API_CANCEL_TRANSACTION = "https://shapeshift.io/cancelpending";

/// Rate (api_rate)

struct api_rate_obj
{
	string coin_pair;				/// as "pair" in JSON
	double rate;
	string error;
};

api_rate_obj
api_rate(string coin_pair);

/// Deposit Limit (api_depositLimit)

struct api_depositLimit_obj
{
	string coin_pair;				/// "pair"
	double limit_max;				/// "limit"
	double limit_min;				/// "min"
	string error;
};

api_depositLimit_obj
api_depositLimit(string coin_pair);

/// Market Info (api_marketInfo)

struct api_marketInfo_obj
{
	string coin_pair;				/// "pair"
	double rate;
	double limit_min;				/// "min"
	double limit_qmax;				/// "limit"; applies only for "quick" tx's
	double limit_max;				/// "maxLimit"
	double minerfee;
	string error;
};

vector<api_marketInfo_obj>
api_marketInfo(string coin_pair);

/// Recent Transaction List (api_recentTransactions)

struct api_recentTransactions_obj
{
	string coin_in;
	string coin_out;
	double amount;
	double timestamp_sec;
	string error;
};

vector<api_recentTransactions_obj>
api_recentTransactions(uint8_t amount = 5);

/// Status of deposit to address (api_transactionStatus)

struct api_transactionStatus_obj
{
	string status;
	string address_in;				/// Used as arg; i.e. deposit address
	string address_out;				/// I.e. withdraw
	string coin_in;
	double amount_in;
	string coin_out;
	double amount_out;
	string tx_id;
	string error;
};

api_transactionStatus_obj
api_transactionStatus(string address_in);

/// Time Remaining on Fixed Amount Transaction (api_timeRemaining)

struct api_timeRemaining_obj
{
	string status;
	int seconds_remaining;
	string error;
};

api_timeRemaining_obj
api_timeRemaining(string deposit_address);

/// Get List of Supported Coins with Icon Links (api_listCoins)

struct api_listCoins_obj
{
	string name;
	string symbol;
	string url_image;
	string status;
	string error;
};

vector<api_listCoins_obj>
api_listCoins();

/// Get List of Transactions with a PRIVATE API KEY
/// (api_listTransactions_private)
/// Get List of Transactions with a Specific Output Address
/// (api_listTransactions_private, overloaded function)

struct api_listTransactions_private_obj
{
	string tx_id_in;
	string address_in;
	string coin_in;
	double amount_in;
	string tx_id_out;
	string address_out;
	string coin_out;
	double amount_out;
	double rate;
	string status;
	string error;
};

vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key);

vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key, string address_out);

/// Validate an address, given a currency symbol and address.
/// (api_validateAddress)

struct api_validateAddress_obj
{
	bool isValid;
	string error;
};

api_validateAddress_obj
api_validateAddress(string address, string coin);

/// Normal Transaction (api_createTransaction_quick)

struct api_createTransaction_quick_obj
{
	string order_id;
	string address_in;
	string coin_in;
	string address_out;
	string coin_out;
	string nxt_public_key;
	string xrp_destination_tag;
	string api_public_key;
	string error;
};

api_createTransaction_quick_obj
api_createTransaction_quick(string address_out, string coin_pair,
	string return_address = "", string api_public_key = "");

/// Request Email Receipt (api_requestEmailReceipt)

struct api_requestEmailReceipt_obj
{
	string status;
	string message;
	string error;
};

api_requestEmailReceipt_obj
api_requestEmailReceipt(string email_address, string tx_id);

/// Fixed Amount Transaction (api_createTransaction)
/// Quote Send Exact Price (api_createTransaction, overloaded function)

struct api_createTransaction_obj
{
	string order_id;
	string coin_pair;
	string address_in;
	double amount_in;
	string address_out;
	double amount_out;
	string return_address;
	uint64_t expiration;
	double rate_fixed;
	double limit_max;
	double miner_fee;
	string api_public_key;
	string error;
};

api_createTransaction_obj
api_createTransaction(string address_out, double amount, string coin_pair,
	string return_address = "", string api_public_key = "");

api_createTransaction_obj
api_createTransaction(double amount, string coin_pair);

/// Cancel Pending Transaction (api_cancelTransaction)

struct api_cancelTransaction_obj
{
	string success;
	string error;
};

api_cancelTransaction_obj
api_cancelTransaction(string address_in);

#endif // SHAPESHIFT_API_INCLUDED
