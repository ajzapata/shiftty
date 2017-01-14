#ifndef SHAPESHIFT_API_INCLUDED
#define SHAPESHIFT_API_INCLUDED

#include <cstdlib>
#include <string>
#include <vector>
#include <cinttypes>
#include <thread>
#include <chrono>

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

/* Shapeshift API Objects - Encapsulates API response */

struct api_rate_obj
{
	string coin_pair;				/// as "pair" in JSON
	double rate;
	string error;
};

struct api_depositLimit_obj
{
	string coin_pair;				/// "pair"
	double limit_max;				/// "limit"
	double limit_min;				/// "min"
	string error;
};

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

struct api_recentTransactions_obj
{
	string coin_in;
	string coin_out;
	double amount;
	double timestamp_sec;
	string error;
};

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

struct api_timeRemaining_obj
{
	string status;
	int seconds_remaining;
	string error;
};

struct api_listCoins_obj
{
	string name;
	string symbol;
	string url_image;
	string status;
	string error;
};

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

struct api_validateAddress_obj
{
	bool isValid;
	string error;
};

struct api_createTransaction_quick_obj
{
	string address_in;
	string coin_in;
	string address_out;
	string coin_out;
	string nxt_public_key;
	string xrp_destination_tag;
	string api_public_key;
	string error;
};

struct api_requestEmailReceipt_obj
{
	string status;
	string message;
	string error;
};

struct api_createTransaction_obj
{
	string coin_pair;
	string address_in;
	double amount_in;
	string address_out;
	double amount_out;
	uint64_t expiration;
	double rate_fixed;
	double miner_fee;
	string api_public_key;
	string error;
};

struct api_cancelTransaction_obj
{
	string success;
	string error;
};

/* Shapeshift API Function Declarations */

api_rate_obj api_rate(string coin_pair);

api_depositLimit_obj api_depositLimit(string coin_pair);

vector<api_marketInfo_obj> api_marketInfo(string coin_pair);

vector<api_recentTransactions_obj> api_recentTransactions(uint8_t amount = 5);

api_transactionStatus_obj api_transactionStatus(string address_in);

api_timeRemaining_obj api_timeRemaining(string deposit_address);

vector<api_listCoins_obj> api_listCoins();

vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key);

vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key, string address_out);

api_validateAddress_obj api_validateAddress(string address, string coin);

#if 0

api_createTransaction_quick_obj
api_createTransaction_quick(string address_out, string coin_pair,
	string return_address = "", string api_public_key = "");

api_requestEmailReceipt_obj
api_requestEmailReceipt(string email_address, string tx_id);

api_createTransaction_obj
api_createTransaction(string address_out, double amount, string coin_pair,
	bool getQuoteOnly = false, string return_address = "",
	string api_public_key = "");

api_cancelTransaction_obj api_cancelTransaction(string address_in);

#endif // 0

#endif // SHAPESHIFT_API_INCLUDED
