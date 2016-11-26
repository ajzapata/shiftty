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
	double d_rate = strtod(s_rate.c_str(), NULL);

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
	double d_min = strtod(s_min.c_str(), NULL);
	double d_max = strtod(s_max.c_str(), NULL);

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

	/// Similarly, the JSON object must be interpreted differently to account
	/// for these "duplicate" pairs. That is, each coin pair is stored in the
	/// object as duplicate values (mostly due to how the code was
	/// implemented), so the getItem function isn't usable. So the values are
	/// manually retrieved in order, with "name" values first and "value"
	/// values immediately following.

	/// The Shapeshift API responds differently when given a valid coin pair,
	/// an invalid coin pair, and no argument. The order of fields differ
	/// between the output of a coin-pair versus giving no argument, and the
	/// output of an invalid coin-pair only has an "error" field. We check for
	/// this by first evaluating the presence of an "error" field, then
	/// checking for duplicate fields (i.e. having a large amount of pairs).

	JSON_item i_pair, i_rate, i_minerFee, i_min, i_max, i_qmax, i_error;
	string s_pair;
	double d_rate, d_minerFee, d_min, d_max, d_qmax;
	api_marketInfo_obj obj;
	if (json_marketInfo.getItem("error") != JSON_ITEM_EMPTY) /// Invalid pair
	{
		i_error = json_marketInfo.getItem("error");

		/// No need to define the other values, since the error message should
		/// be checked first before attempting to read the other valies.
		obj.error = i_error.value();
		v_obj.push_back(obj);
	}
	else if (json_marketInfo.items().size() > 6) /// No argument given
	{
		for (size_t i = 0; i < json_marketInfo.items().size(); i++)
		{
			/// Iterate through the item-pairs and increment iterator as we go.
			/// This is safe as long as there are no uneven pairs in the API
			/// response.

			i_rate = json_marketInfo.items()[i++];
			i_qmax = json_marketInfo.items()[i++];
			i_pair = json_marketInfo.items()[i++];
			i_max = json_marketInfo.items()[i++];
			i_min = json_marketInfo.items()[i++];
			i_minerFee = json_marketInfo.items()[i]; // incr. on loop-continue

			/// Interpret item-pairs where necessary
			s_pair = i_pair.value();
			d_rate = strtod(i_rate.value().c_str(), NULL);
			d_minerFee = strtod(i_minerFee.value().c_str(), NULL);
			d_min = strtod(i_min.value().c_str(), NULL);
			d_max = strtod(i_max.value().c_str(), NULL);
			d_qmax = strtod(i_qmax.value().c_str(), NULL);

			/// Fill API object fields
			obj.coin_pair = s_pair;
			obj.rate = d_rate;
			obj.limit_min = d_min;
			obj.limit_qmax = d_qmax;
			obj.limit_max = d_max;
			obj.minerfee = d_minerFee;

			v_obj.push_back(obj);
		}
	}
	else /// Valid pair
	{
		/// Retrieve item-pairs

		i_pair = json_marketInfo.items()[0];
		i_rate = json_marketInfo.items()[1];
		i_minerFee = json_marketInfo.items()[2];
		i_qmax = json_marketInfo.items()[3];
		i_min = json_marketInfo.items()[4];
		i_max = json_marketInfo.items()[5];

		/// Interpret item-pairs where necessary
		s_pair = i_pair.value();
		d_rate = strtod(i_rate.value().c_str(), NULL);
		d_minerFee = strtod(i_minerFee.value().c_str(), NULL);
		d_min = strtod(i_min.value().c_str(), NULL);
		d_max = strtod(i_max.value().c_str(), NULL);
		d_qmax = strtod(i_qmax.value().c_str(), NULL);

		/// Fill API object fields
		obj.coin_pair = s_pair;
		obj.rate = d_rate;
		obj.limit_min = d_min;
		obj.limit_qmax = d_qmax;
		obj.limit_max = d_max;
		obj.minerfee = d_minerFee;

		v_obj.push_back(obj);
	}

    return v_obj;
}

vector<api_recentTransactions_obj> api_recentTransactions(uint8_t amount)
{
	/// Amount must be between 1 and 50 inclusive.
	if (amount < 1 || amount > 50)
	{
		/// Return custom error message for API call to fail safely
		api_recentTransactions_obj obj;
		obj.error = "[SELF] Transaction amount out of range.";
		vector<api_recentTransactions_obj> v_obj;
		v_obj.push_back(obj);
		return v_obj;
	}

	/// API call
	string json_recentTransactions_raw =
		http_get(URL_API_RECENT_TRANSACTIONS + to_string(amount));

	/// Interpret and extract JSON data
	JSON json_recentTransactions;
	json_recentTransactions.importRaw(json_recentTransactions_raw);

	vector<api_recentTransactions_obj> v_obj;

	/// Check for error field first
	if (json_recentTransactions.getItem("error") != JSON_ITEM_EMPTY)
	{
		api_recentTransactions_obj obj;
		obj.error = json_recentTransactions.getItem("error").value();
		v_obj.push_back(obj);
		return v_obj;
	}

	JSON_item i_curIn, i_curOut, i_timestamp, i_amount;
	string s_curIn, s_curOut;
	double d_timestamp, d_amount;
	api_recentTransactions_obj obj;
	for (size_t i = 0; i < json_recentTransactions.items().size(); i++)
	{
		/// Retrieve item-pairs
		i_curIn = json_recentTransactions.items()[i++];
		i_curOut = json_recentTransactions.items()[i++];
		i_timestamp = json_recentTransactions.items()[i++];
		i_amount = json_recentTransactions.items()[i];

		/// Interpret item-pairs where necessary
		s_curIn = i_curIn.value();
		s_curOut = i_curOut.value();
		d_timestamp = strtod(i_timestamp.value().c_str(), NULL);
		d_amount = strtod(i_amount.value().c_str(), NULL);

		/// Fill API object fields
		obj.coin_in = s_curIn;
		obj.coin_out = s_curOut;
		obj.timestamp_sec = d_timestamp;
		obj.amount = d_amount;

		v_obj.push_back(obj);
	}

	return v_obj;
}

#if 0

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
