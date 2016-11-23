#include "shapeshift-api.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cinttypes>
#include <cassert>
#include "http.h"
#include "json.h"

/// TODO:
/// - Optimize code (e.g. to pass pointers rather than objects)

using namespace std;

char* to_cstr(string s) // s has member func s.c_str()
{
	char* cstr = new char[s.size() + 1];

	strcpy(cstr, s.c_str());
	return cstr;
}

api_rate_obj api_rate(string coin_pair)
{
	/// API call
	string json_rate_raw = http_get(URL_API_RATE + coin_pair);

	/// Interpret and extract JSON data
	JSON json_rate;
	json_rate.importRaw(json_rate_raw);

	string s_pair, s_rate, s_error;

	s_pair = json_rate.getItem("pair").value();
	s_rate = json_rate.getItem("rate").value();
	s_error = json_rate.getItem("error").value();

	/// Convert rate from string to double
	double d_rate = atod(s_rate.c_str());

	//delete cs_rate;

	/// Create API object
	api_rate_obj obj;
	obj.coin_pair = s_pair;
	obj.rate = d_rate;
	obj.error = s_error;

	return obj;
}

api_depositLimit_obj api_depositLimit(string coin_pair)
{
	/// API call
	string json_depositLimit_raw =
		http_get(URL_API_DEPOSIT_LIMIT + coin_pair);

	/// Interpret and extract JSON data
	JSON json_depositLimit;
	json_depositLimit.importRaw(json_depositLimit_raw);

	string s_pair, s_min, s_max, s_error;

	s_pair = json_depositLimit.getItem("pair").value();
	s_min = json_depositLimit.getItem("min").value();
	s_max = json_depositLimit.getItem("limit").value();
	s_error = json_depositLimit.getItem("error").value();

	/// Convert string-doubles to doubles
	double d_min = atod(s_min.c_str());
	double d_max = atod(s_max.c_str());

	/// Create API object
	api_depositLimit_obj obj;
	obj.coin_pair = s_pair;
	obj.limit_min = d_min;
	obj.limit_max = d_max;
	obj.error = s_error;

	return obj;
}

vector<api_marketInfo_obj> api_marketInfo(string coin_pair)
{
	/// API call
	string json_marketInfo_raw =
		http_get(URL_API_MARKET_INFO + coin_pair);

	/// Interpret and extract JSON data
	JSON json_marketInfo;
	json_marketInfo.importRaw(json_marketInfo_raw);

    /// Unlike other API calls, the JSON object returned can have multiple
    /// coin-pairs if coin_pair string is not given. So we treat everything as
    /// a vector of api_marketInfo_obj objects

    vector<api_marketInfo_obj> v_obj;



    return v_obj;
}

#if 0

vector<api_recentTransactions_obj> api_recentTransactions(uint8_t amount)
{
	vector<api_recentTransactions_obj> tx_list;

	/// Amount must be between 1 and 50 inclusive; otherwise return empty.
	if (amount < 1 || amount > 50)
		return tx_list;


}

api_transactionStatus_obj api_transactionStatus(string address_in)
{

}

api_timeRemaining_obj api_timeRemaining(string deposit_address)
{

}

vector<api_listCoins_obj> api_listCoins()
{

}

vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key, string address_in)
{

}

api_validateAddress_obj api_validateAddress(string address)
{

}

api_createTransaction_quick_obj
api_createTransaction_quick(string address_out, string coin_pair,
	string return_address, string api_public_key)
{

}

api_recentTransactions_obj
api_recentTransactions(string email_address, string tx_id)
{

}

api_createTransaction_obj
api_createTransaction(string address_out, double amount, string coin_pair,
	bool getQuoteOnly, string return_address, string api_public_key)
{

}

api_cancelTransaction_obj api_cancelTransaction(string address_in)
{

}

#endif // 0
