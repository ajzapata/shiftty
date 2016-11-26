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

/// API call descriptions are from Shapeshift's API documentation at
///    https://info.shapeshift.io/api
/// Descriptions retrieved at Friday November 25, 2016 22:37

/// Rate (api_rate):
/// Gets the current rate offered by Shapeshift. This is an estimate because
/// the rate can occasionally change rapidly depending on the markets. The
/// rate is also a 'use-able' rate not a direct market rate. Meaning
/// multiplying your input coin amount times the rate should give you a close
/// approximation of what will be sent out. This rate does not include the
/// transaction (miner) fee taken off every transaction.
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

/// Deposit Limit (api_depositLimit):
/// Gets the current deposit limit set by Shapeshift. Amounts deposited over
/// this limit will be sent to the return address if one was entered,
/// otherwise the user will need to contact ShapeShift support to retrieve
/// their coins. This is an estimate because a sudden market swing could move
/// the limit.
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

/// Market Info (api_marketInfo):
/// This gets the market info (pair, rate, limit, minimum limit, miner fee)
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
	else if (json_marketInfo.items().size() > 6) /// No argument given (SLOW!)
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

/// Recent Transaction List (api_recentTransactions):
/// Get a list of the most recent transactions.
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

/// Status of deposit to address (api_transactionStatus):
/// This returns the status of the most recent deposit transaction to the
/// address.
api_transactionStatus_obj api_transactionStatus(string address_in)
{
	/// API call
	string json_transactionStatus_raw =
		http_get(URL_API_TRANSACTION_STATUS + address_in);

	/// Interpret and extract JSON data
	JSON json_transactionStatus;
	json_transactionStatus.importRaw(json_transactionStatus_raw);

	api_transactionStatus_obj obj;

	/// The API responds differently depending on the current status of the
	/// transactions. In all cases, the "status" item is present, so we use
	/// this to determine how to proceed (except regular errors).

	JSON_item i_status, i_address, i_withdraw, i_incomingCoin, i_incomingType,
		i_outgoingCoin, i_outgoingType, i_transaction, i_error;
	string s_status, s_address, s_withdraw, s_incomingType, s_outgoingType,
		s_transaction, s_error;
	double d_incomingCoin, d_outgoingCoin;

	/// Safe to find pairs even if they aren't present in the API response;
	/// they would resolve to empty strings.
	i_status = json_transactionStatus.getItem("status");
	i_address = json_transactionStatus.getItem("address");
	i_withdraw = json_transactionStatus.getItem("withdraw");
	i_incomingCoin = json_transactionStatus.getItem("incomingCoin");
	i_incomingType = json_transactionStatus.getItem("incomingType");
	i_outgoingCoin = json_transactionStatus.getItem("outgoingCoin");
	i_outgoingType = json_transactionStatus.getItem("outgoingType");
	i_transaction = json_transactionStatus.getItem("transaction");
	i_error = json_transactionStatus.getItem("error");

	/// Interpret item-pairs as strings (which they already are, but still...)
	s_status = i_status.value();
	s_address = i_address.value();
	s_withdraw = i_withdraw.value();
	s_incomingType = i_incomingType.value();
	s_outgoingType = i_outgoingType.value();
	s_transaction = i_transaction.value();
	s_error = i_error.value();

	/// Interpret item-pairs as doubles; checks are added just in case.
    d_incomingCoin = i_incomingCoin == JSON_ITEM_EMPTY ? -1.00 :
		strtod(i_incomingCoin.value().c_str(), NULL);
	d_outgoingCoin = i_outgoingCoin == JSON_ITEM_EMPTY ? -1.00 :
		strtod(i_outgoingCoin.value().c_str(), NULL);

	/// First check for "error" field WITHOUT a status field (indicating a
	/// "classic" error, such as providing no argument (empty-string)).
	if (i_error != JSON_ITEM_EMPTY && i_status == JSON_ITEM_EMPTY)
	{
		obj.error = s_error;
	}
	else if (s_status == "no_deposits") /// No deposits received
	{
        obj.status = s_status;
        obj.address_in = s_address;
	}
	else if (s_status == "received") /// Deposit received & is being processed
	{
        obj.status = s_status;
        obj.address_in = s_address;
	}
	else if (s_status == "complete") /// Transaction complete
	{
		obj.status = s_status;
		obj.address_in = s_address;
		obj.address_out = s_withdraw;
		obj.coin_in = s_incomingType;
		obj.amount_in = d_incomingCoin;
		obj.coin_out = s_outgoingType;
		obj.amount_out = d_outgoingCoin;
		obj.tx_id = s_transaction;
	}
	else if (s_status == "failed" || s_status == "error") /// Tx failed
	{
		obj.status = s_status; // Can also be "error"; this was undocumented
		obj.error = s_error;
	}

	return obj;
}

/// Time Remaining on Fixed Amount Transaction (api_timeRemaining):
/// When a transaction is created with a fixed amount requested there is a 10
/// minute window for the deposit. After the 10 minute window if the deposit
/// has not been received the transaction expires and a new one must be
/// created. This api call returns how many seconds are left before the
/// transaction expires. Please note that if the address is a ripple address,
/// it will include the "?dt=destTagNUM" appended on the end, and you will
/// need to use the URIEncodeComponent() function on the address before
/// sending it in as a param, to get a successful response.
api_timeRemaining_obj api_timeRemaining(string deposit_address)
{
	/// API call
	string json_timeRemaining_raw =
		http_get(URL_API_TIME_REMAINING + deposit_address);

	/// Interpret and extract JSON data
	JSON json_timeRemaining;
	json_timeRemaining.importRaw(json_timeRemaining_raw);

	api_timeRemaining_obj obj;

	JSON_item i_status = json_timeRemaining.getItem("status");
	JSON_item i_seconds = json_timeRemaining.getItem("seconds_remaining");
	JSON_item i_error = json_timeRemaining.getItem("error");

	if (i_error != JSON_ITEM_EMPTY)
	{
		obj.error = i_error.value();
	}
	else
	{
		obj.status = i_status.value();
		obj.seconds_remaining = atoi(i_seconds.value().c_str());
	}

	return obj;
}

#if 0

/// Get List of Supported Coins with Icon Links (api_listCoins):
/// Allows anyone to get a list of all the currencies that Shapeshift
/// currently supports at any given time. The list will include the name,
/// symbol, availability status, and an icon link for each.
vector<api_listCoins_obj> api_listCoins()
{

}

/// Get List of Transactions with a PRIVATE API KEY
/// (api_listTransactions_private):
/// Allows vendors to get a list of all transactions that have ever been done
/// using a specific API key. Transactions are created with an affilliate
/// PUBLIC KEY, but they are looked up using the linked PRIVATE KEY, to
/// protect the privacy of our affiliates' account details.
vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key, string address_in)
{

}

/// Get List of Transactions with a Specific Output Address (MISSING!)
/// Allows vendors to get a list of all transactions that have ever been sent
/// to one of their addresses. The affilliate's PRIVATE KEY must be provided,
/// and will only return transactions that were sent to output address AND
/// were created using / linked to the affiliate's PUBLIC KEY. Please note
/// that if the address is a ripple address and it includes the
/// "?dt=destTagNUM" appended on the end, you will need to use the
/// URIEncodeComponent() function on the address before sending it in as a
/// param, to get a successful response.

/// Validate an address, given a currency symbol and address.
/// (api_validateAddress):
/// Allows user to verify that their receiving address is a valid address
/// according to a given wallet daemon. If isvalid returns true, this address
/// is valid according to the coin daemon indicated by the currency symbol.
api_validateAddress_obj api_validateAddress(string address)
{

}

/// Normal Transaction (api_createTransaction_quick): *(not sure if quick?)
/// This is the primary data input into ShapeShift.
api_createTransaction_quick_obj
api_createTransaction_quick(string address_out, string coin_pair,
	string return_address, string api_public_key)
{

}

/// Request Email Receipt (api_requestEmailReceipt):
/// This call requests a receipt for a transaction. The email address will be
/// added to the conduit associated with that transaction as well. (Soon it
/// will also send receipts to subsequent transactions on that conduit)
api_requestEmailReceipt_obj
api_requestEmailReceipt(string email_address, string tx_id)
{

}

/// Fixed Amount Transaction / Quote Send Exact Price (api_createTransaction):
/// This call allows you to request a fixed amount to be sent to the withdrawal
/// address. You provide a withdrawal address and the amount you want sent to
/// it. We return the amount to deposit and the address to deposit to. This
/// allows you to use shapeshift as a payment mechanism. This call also allows
/// you to request a quoted price on the amount of a transaction without a
/// withdrawal address.
api_createTransaction_obj
api_createTransaction(string address_out, double amount, string coin_pair,
	bool getQuoteOnly, string return_address, string api_public_key)
{

}

/// Cancel Pending Transaction (api_cancelTransaction):
/// This call allows you to request for canceling a pending transaction by the
/// deposit address. If there is fund sent to the deposit address, this
/// pending transaction cannot be canceled.
api_cancelTransaction_obj api_cancelTransaction(string address_in)
{

}

#endif // 0
