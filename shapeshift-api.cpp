#include "shapeshift-api.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cinttypes>
#include <cassert>
#include <iostream>
#include "http.h"
#include "json/json.h"
#include "json/json-forwards.h"

/// TODO:
/// - Optimize code (e.g. to pass pointers rather than objects)
/// - Encode ripple addresses (at the moment, they're not compatible since the
///   URI encoding component hasn't been implemented yet.

using namespace std;

const bool SUPPRESS_LOG_MESSAGES = false;

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
	string json_data_raw = http_get(URL_API_RATE + coin_pair);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	/// Create API object
	api_rate_obj obj;
	if (json_data["error"].asString().empty())
	{
		/// Although JsonCpp uses default values (as "null values") when an
		/// non-existant array element is dereferenced, in the cases where
		/// type-conversion is done manually (e.g. "rate" below), the default
		/// value is incompatible across types.
		obj.coin_pair = json_data["pair"].asString();
		obj.rate = strtod(json_data["rate"].asCString(), NULL);
			/// Using JsonCpp's Value::asDouble() doesn't work for "rate" since
			/// the decimal value is quoted in the source JSON and thus
			/// considered a string.
	}
	obj.error = json_data["error"].asString();

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
	string json_data_raw = http_get(URL_API_DEPOSIT_LIMIT + coin_pair);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	/// Create API object
	api_depositLimit_obj obj;
	if (json_data["error"].asString().empty())
	{
		obj.coin_pair = json_data["pair"].asString();
		obj.limit_min = strtod(json_data["min"].asCString(), NULL);
		obj.limit_max = strtod(json_data["limit"].asCString(), NULL);
	}
	obj.error = json_data["error"].asString();

	return obj;
}

/// Market Info (api_marketInfo):
/// This gets the market info (pair, rate, limit, minimum limit, miner fee)
vector<api_marketInfo_obj> api_marketInfo(string coin_pair)
{
	/// Calling this function without an argument (the empty string) will
	/// return multiple coin-pairs and may take a significant time to process;
	/// thus, a warning is provided.
	if (coin_pair == "" && !SUPPRESS_LOG_MESSAGES) cerr <<
		"api_marketInfo called with empty string; this may take a while..."
		<< endl;

	/// API call
	string json_data_raw =
		http_get(URL_API_MARKET_INFO + coin_pair);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

    /// Unlike other API calls, the JSON object returned can have multiple
    /// coin-pairs if coin_pair string is not given. So we treat everything as
    /// a vector of api_marketInfo_obj objects

    vector<api_marketInfo_obj> v_obj;

	/// The Shapeshift API responds differently when given a valid coin pair,
	/// an invalid coin pair, and no argument. The order of fields differ
	/// between the output of a coin-pair versus giving no argument, and the
	/// output of an invalid coin-pair only has an "error" field. We check for
	/// this by first evaluating the presence of an "error" field, then
	/// checking for duplicate fields (i.e. having a large amount of pairs).

	/// JSON: objects -> {...}, arrays -> [...], array of objects -> [{...}]

	api_marketInfo_obj obj;
	if (json_data.isObject() && json_data["error"].asString() != "") /// Error
	{
		/// No need to define the other values, since the error message should
		/// be checked first before attempting to read the other values.
		obj.error = json_data["error"].asString();
		v_obj.push_back(obj);
	}
	else if (json_data.isArray()) /// No argument given (SLOW!)
	{
		/// When no argument is given, the API returns an array of objects
		/// representing a list of all recognized coin-pairs. For any other
		/// argument, the API returns a specified coin-pair object or an object
		/// with only an error response.

		/// Must use Json::Value::ArrayIndex as iterator type; otherwise, you
		/// get an "ambiguous operator[]" error

		for (Json::Value::ArrayIndex i = 0; i < json_data.size(); i++)
		{
			/// Fill API object fields
			obj.coin_pair = json_data[i]["pair"].asString();
			obj.rate = strtod(json_data[i]["rate"].asCString(), NULL);
				/// In this case, "rate" is a quoted decimal number
			obj.limit_min = json_data[i]["min"].asDouble();
			obj.limit_qmax = json_data[i]["maxLimit"].asDouble();
			obj.limit_max = json_data[i]["limit"].asDouble();
			obj.minerfee = json_data[i]["minerFee"].asDouble();

			v_obj.push_back(obj);
		}
	}
	else /// Valid pair
	{
		/// Fill API object fields
		obj.coin_pair = json_data["pair"].asString();
		obj.rate = json_data["rate"].asDouble();
			/// Unlike some API calls, the decimal numbers here aren't quoted
			/// in the source JSON, so they can be directly cast to the double
			/// type as done by Value::asDouble().
		obj.limit_min = json_data["minimum"].asDouble();
		obj.limit_qmax = json_data["limit"].asDouble();
		obj.limit_max = json_data["maxLimit"].asDouble();
		obj.minerfee = json_data["minerFee"].asDouble();

		v_obj.push_back(obj);
	}

    return v_obj;
}

/// Recent Transaction List (api_recentTransactions):
/// Get a list of the most recent transactions.
vector<api_recentTransactions_obj> api_recentTransactions(uint8_t amount)
{
	/// Amount should be between 1 and 50 inclusive. In reality, the API
	/// rounds the given amount to the nearest valid number (as of January 6,
	/// 2017), but to maintain consistency in case this behavior changes, this
	/// function does the rounding automatically before the API call is made.
	/// This API call never returns an error message either(?); negative or
	/// non-numeric input is replaced by the default amount of 5 (not possible
	/// to input in this function and thus not handled).
	if (amount < 1 && !SUPPRESS_LOG_MESSAGES) {
		cerr <<
			"api_recentTransactions: amount < 1. Defaulting to amount = 5"
			<< endl;
		amount = 5;
	}
	else if (amount > 50 && !SUPPRESS_LOG_MESSAGES) {
		cerr <<
			"api_recentTransactions: amount > 50. Defaulting to amount = 50"
			<< endl;
		amount = 50;
	}

	/// API call
	string json_data_raw =
		http_get(URL_API_RECENT_TRANSACTIONS + to_string(amount));

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	vector<api_recentTransactions_obj> v_obj;

	api_recentTransactions_obj obj;

	/// Check for potential API error object. There's no reason to believe they
	/// exist in this form, but it's better than finding out the hard way...
	if (json_data.isObject() && !json_data["error"].asString().empty())
	{
		obj.error = json_data["error"].asString();
		v_obj.push_back(obj);
		return v_obj;
	}
	else if (json_data.isArray() && !json_data[0]["error"].asString().empty())
	{
		obj.error = json_data[0]["error"].asString();
		v_obj.push_back(obj);
		return v_obj;
	}

	for (Json::Value::ArrayIndex i = 0; i < json_data.size(); i++)
	{
		/// Regardless of amount, the source JSON has the transaction objects
		/// in an array ("array of objects" form).

		/// Fill API object fields
		obj.coin_in = json_data[i]["curIn"].asString();
		obj.coin_out = json_data[i]["curOut"].asString();
		obj.timestamp_sec = json_data[i]["timestamp"].asDouble();
		obj.amount = json_data[i]["amount"].asDouble();

		v_obj.push_back(obj);
	}

	return v_obj;
}

/// Status of deposit to address (api_transactionStatus):
/// This returns the status of the most recent deposit transaction to the
/// address.
/// TODO: Test using live data (currently untested using live data)
api_transactionStatus_obj api_transactionStatus(string address_in)
{
	/// API call
	string json_data_raw =
		http_get(URL_API_TRANSACTION_STATUS + address_in);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_transactionStatus_obj obj;

	/// The API responds differently depending on the current status of the
	/// transactions. In most cases, the "status" item is present, so we use
	/// this to determine how to proceed.

	/// TODO: Optimize (e.g. switch statement using enum, less repetition)
	if (json_data.size() == 1)
	{
		/// Error without a "status" field; it is assumed to be the only item.
		obj.error = json_data["error"].asString();
	}
	else if (json_data["status"].asString() == "no_deposits")
	{
		/// No deposits received
        obj.status = json_data["status"].asString();
        obj.address_in = json_data["address"].asString();
	}
	else if (json_data["status"].asString() == "received")
	{
		/// Deposit received and is being processed
        obj.status = json_data["status"].asString();
        obj.address_in = json_data["address"].asString();
	}
	else if (json_data["status"].asString() == "complete")
	{
		/// Transaction complete
		/// TODO: Look at results from live tests to see if decimal quantities
		/// are strings (in quotes) or doubles, in order to replace the
		/// currently-used expressions
        obj.status = json_data["status"].asString();
        obj.address_in = json_data["address"].asString();
		obj.address_out = json_data["withdraw"].asString();
		obj.coin_in = json_data["incomingType"].asString();
		obj.amount_in = json_data["incomingCoin"].isString() ?
			strtod(json_data["incomingCoin"].asCString(), NULL) :
			json_data["incomingCoin"].asDouble();
		obj.coin_out = json_data["outgoingType"].asString();
		obj.amount_out = json_data["outgoingCoin"].isString() ?
			strtod(json_data["outgoingCoin"].asCString(), NULL) :
			json_data["outgoingCoin"].asDouble();
		obj.tx_id = json_data["transaction"].asString();
	}
	else if (json_data["status"].asString() == "failed")
	{
		/// Transaction failed
        obj.status = json_data["status"].asString();
		obj.error = json_data["error"].asString();
	}
	else if (json_data["status"].asString() == "error")
	{
		/// General error; this was undocumented and seems to be in place of
		/// the generic one-item "error" API response, with the addition of a
		/// more descriptive error message (e.g. to not send coins to the given
		/// invalid address) and the address as-interpreted by the API.
		obj.status = json_data["status"].asString();
		obj.address_in = json_data["address"].asString();
		obj.error = json_data["error"].asString();
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
api_timeRemaining_obj api_timeRemaining(string deposit_address) /// TODO: TEST
{
	/// API call
	string json_data_raw =
		http_get(URL_API_TIME_REMAINING + deposit_address);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_timeRemaining_obj obj;

	if (json_data.size() == 1) /// Error
	{
		obj.error = json_data["error"].asString();
	}
	else
	{
		obj.status = json_data["status"].asString();
		obj.seconds_remaining = json_data["seconds_remaining"].asInt();
	}

	return obj;
}

/// Get List of Supported Coins with Icon Links (api_listCoins):
/// Allows anyone to get a list of all the currencies that Shapeshift
/// currently supports at any given time. The list will include the name,
/// symbol, availability status, and an icon link for each.
vector<api_listCoins_obj> api_listCoins()
{
	/// The JSON output from the API call is in the form of an object where
	/// each entry represents a supported currency and is paired with an object
	/// containing information about it. The format of the output is unique in
	/// that the names of the fields are part of the useful information being
	/// retrieved and vary as much as the content they reference. Thus,
	/// indexing is limited to evaluating the size of the object and
	/// accounting for each entry. The current data-model used to represent
	/// API output (i.e. structs and vectors of structs) treats this format
	/// similar to "array of objects".

	/// API call
	string json_data_raw = http_get(URL_API_LIST_COINS);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	vector<api_listCoins_obj> v_obj;

	api_listCoins_obj obj;

	/// Check for errors
	if (!json_data["error"].asString().empty())
	{
		obj.error = json_data["error"].asString();
		v_obj.push_back(obj);
		return v_obj;
	}

	/// Get names of members and iterate through them
	vector<string> json_data_members = json_data.getMemberNames();
	for (size_t i = 0; i < json_data_members.size(); i++)
	{
		string s = json_data_members[i];

		/// Avoid inserting null members into API-object array
		if (json_data[s].isNull()) continue;

		/// Fill API object fields
		obj.name = json_data[s]["name"].asString();
		obj.symbol = json_data[s]["symbol"].asString();
		obj.url_image = json_data[s]["image"].asString();
		obj.status = json_data[s]["status"].asString();

		v_obj.push_back(obj);
	}

	return v_obj;
}

/// Get List of Transactions with a PRIVATE API KEY
/// (api_listTransactions_private):
/// Allows vendors to get a list of all transactions that have ever been done
/// using a specific API key. Transactions are created with an affilliate
/// PUBLIC KEY, but they are looked up using the linked PRIVATE KEY, to
/// protect the privacy of our affiliates' account details.
vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key) /// TODO: TEST
{
	/// Call the overloaded function with the empty string as the argument for
	/// address_out; the function uses the correct API call as part of the
	/// error checks.
	return api_listTransactions_private(api_key, string());
}

/// Get List of Transactions with a Specific Output Address
/// (api_listTransactions_private, overloaded function):
/// Allows vendors to get a list of all transactions that have ever been sent
/// to one of their addresses. The affilliate's PRIVATE KEY must be provided,
/// and will only return transactions that were sent to output address AND
/// were created using / linked to the affiliate's PUBLIC KEY. Please note
/// that if the address is a ripple address and it includes the
/// "?dt=destTagNUM" appended on the end, you will need to use the
/// URIEncodeComponent() function on the address before sending it in as a
/// param, to get a successful response.
vector<api_listTransactions_private_obj>
api_listTransactions_private(string api_key, string address_out) /// TODO: TEST
{
	/// API call
	/// If any string is the empty string, an HTTP redirect occurs (undefined)
	string json_data_raw;
	if (api_key.empty())
		json_data_raw = "{\"error\":\"[SELF] Empty string\"}";
	else if (address_out.empty()) /// Look up all transactions by key
		json_data_raw =
			http_get(URL_API_LIST_TRANSACTIONS_PRIVATE + api_key);
	else /// Look up transactions by key and filter by outgoing a
		json_data_raw =
			http_get(URL_API_LIST_TRANSACTIONS_PRIVATE_ADDR + api_key + "/" +
				address_out);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	vector<api_listTransactions_private_obj> v_obj;

	api_listTransactions_private_obj obj;

	/// Check for potential API error object. There's no reason to believe they
	/// exist in this form, but it's better than finding out the hard way...
	if (json_data.empty()) /// Empty set (invalid input or no tx found)
	{
		/// This check must be the first since searching for a nonexisting
		/// member internally creates it and assigns it a null value (i.e from
		/// then on, isMember() will return true for that item).
		obj.error = "[SELF] Empty set";
		v_obj.push_back(obj);
		return v_obj;
	}
	else if (json_data.isObject() && !json_data["error"].asString().empty())
	{
		obj.error = json_data["error"].asString();
		v_obj.push_back(obj);
		return v_obj;
	}
	else if (json_data.isArray() &&
		(!json_data[0]["error"].asString().empty() ||
		json_data[0]["status"].asString() == "error")) /// TODO: See error fmt.
	{
		obj.status = json_data[0]["status"].asString();
		obj.error = json_data[0]["error"].asString();
		v_obj.push_back(obj);
		return v_obj;
	}

	for (Json::Value::ArrayIndex i = 0; i < json_data.size(); i++)
	{
		/// Regardless of amount, the source JSON has the transaction objects
		/// in an array ("array of objects" form).
		/// TODO: Look at results from live tests to see if decimal quantities
		/// are strings (in quotes) or doubles, in order to replace the
		/// currently-used expressions

		/// Fill API object fields
		obj.tx_id_in = json_data[i]["inputTXID"].asString();
		obj.address_in = json_data[i]["inputAddress"].asString();
		obj.coin_in = json_data[i]["inputCurrency"].asString();
		obj.amount_in = json_data[i]["inputAmount"].isString() ?
			strtod(json_data[i]["inputAmount"].asCString(), NULL) :
			json_data[i]["inputAmount"].asDouble();
		obj.tx_id_out = json_data[i]["outputTXID"].asString();
		obj.address_out = json_data[i]["outputAddress"].asString();
		obj.coin_out = json_data[i]["outputCurrency"].asString();
		obj.amount_out = json_data[i]["outputAmount"].isString() ?
			strtod(json_data[i]["outputAmount"].asCString(), NULL) :
			json_data[i]["outputAmount"].asDouble();
		obj.rate = json_data[i]["shiftRate"].isString() ?
			strtod(json_data[i]["shiftRate"].asCString(), NULL) :
			json_data[i]["shiftRate"].asDouble();
		obj.status = json_data[i]["status"].asString();

		v_obj.push_back(obj);
	}

	return v_obj;
}

/// Validate an address, given a currency symbol and address.
/// (api_validateAddress):
/// Allows user to verify that their receiving address is a valid address
/// according to a given wallet daemon. If isvalid returns true, this address
/// is valid according to the coin daemon indicated by the currency symbol.
api_validateAddress_obj api_validateAddress(string address, string coin)
{
	/// API call
	/// If the address and/or coin symbol is the empty string, the API call
	/// will fail (HTTP redirect)
	string json_data_raw = address.empty() || coin.empty() ?
		"{\"isvalid\":false,\"error\":\"[SELF] Empty string in arg\"}" :
		http_get(URL_API_VALIDATE_ADDRESS + address + "/" + coin);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_validateAddress_obj obj;

	/// Fill API object fields
	obj.isValid = json_data.isMember("isvalid") ?
		json_data["isvalid"].asBool() : false;
	obj.error = json_data["error"].isObject() ? /// In source JSON as: {}
		"[SELF] Invalid address and/or coin symbol" :
		json_data["error"].asString();

	return obj;
}

/// Normal Transaction (api_createTransaction_quick):
/// This is the primary data input into ShapeShift.
api_createTransaction_quick_obj
api_createTransaction_quick(string address_out, string coin_pair,
	string return_address, string api_public_key)
{
	/// The first of the API calls that uses HTTP POST rather than HTTP GET.
	/// This requires submitting data along with the HTTP request, then
	/// interpreting the response. The data being submitted is in JSON format.

	Json::Value json_post_data;
	json_post_data["withdrawal"] = address_out;
	json_post_data["pair"] = coin_pair;
	if (!return_address.empty())
		json_post_data["returnAddress"] = return_address;
	if (!api_public_key.empty())
		json_post_data["apiKey"] = api_public_key;

	/// Remove spaces, carriage-returns, and new-lines from styled JSON string
	string json_post_data_str = json_post_data.toStyledString();
	string post_data;
	char c;
	for (size_t i = 0; i < json_post_data_str.length(); i++)
	{
        c = json_post_data_str[i];
        if (c != ' ' && c != '\n' && c != '\r')
			post_data += c;
	}

	/// API call
	string json_data_raw =
		http_post(URL_API_CREATE_TRANSACTION_QUICK,	post_data);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_createTransaction_quick_obj obj;

	/// Fill API object fields
	obj.order_id = json_data["orderId"].asString();
	obj.address_in = json_data["deposit"].asString();
	obj.coin_in = json_data["depositType"].asString();
	obj.address_out = json_data["withdrawal"].asString();
	obj.coin_out = json_data["withdrawalType"].asString();
	obj.nxt_public_key = json_data["public"].asString();
	obj.xrp_destination_tag = json_data["xrpDestTag"].asString();
	obj.api_public_key = json_data["apiPubKey"].asString();
	obj.error = json_data["error"].asString();

	return obj;
}

/// Request Email Receipt (api_requestEmailReceipt):
/// This call requests a receipt for a transaction. The email address will be
/// added to the conduit associated with that transaction as well. (Soon it
/// will also send receipts to subsequent transactions on that conduit)
/// TODO: TEST WITH VALID INPUT
api_requestEmailReceipt_obj
api_requestEmailReceipt(string email_address, string tx_id)
{
	/// Simple enough to write the JSON post data manually
	string post_data =
		"{\"email\":\"" + email_address + "\",\"txid\":\"" + tx_id + "\"}";

	/// API call
	string json_data_raw =
		http_post(URL_API_REQUEST_EMAIL_RECEIPT, post_data);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_requestEmailReceipt_obj obj;

	/// Fill API object fields
	if (json_data.isMember("email") && json_data["email"].isObject())
	{
		obj.status = json_data["email"]["status"].asString();
		obj.message = json_data["email"]["message"].asString();
		obj.error = json_data["email"]["error"].asString();
	}
	else obj.error = json_data["error"].asString();

	return obj;
}

/// Fixed Amount Transaction (api_createTransaction):
/// This call allows you to request a fixed amount to be sent to the withdrawal
/// address. You provide a withdrawal address and the amount you want sent to
/// it. We return the amount to deposit and the address to deposit to. This
/// allows you to use shapeshift as a payment mechanism. ...
/// TODO: TEST WITH VALID INPUT (NOT READY FOR LIVE USE)
api_createTransaction_obj
api_createTransaction(string address_out, double amount, string coin_pair,
	string return_address, string api_public_key)
{
	/// Since the members in the JSON POST data can change depending on the
	/// arguments given, it's more convenient to use the exisiting JSON
	/// framework (originally used to only read API responses) than manually
	/// write a compatible JSON string for each case.
	Json::Value json_post_data;
	json_post_data["withdrawal"] = address_out;
	json_post_data["amount"] = amount; /// VERIFY THAT IT'S A DOUBLE (NOT STR)
	json_post_data["pair"] = coin_pair;
	if (!return_address.empty())
		json_post_data["returnAddress"] = return_address;
	if (!api_public_key.empty())
		json_post_data["apiKey"] = api_public_key;

	/// Remove spaces, carriage-returns, and new-lines from styled JSON string
	string json_post_data_str = json_post_data.toStyledString();
	string post_data;
	char c;
	for (size_t i = 0; i < json_post_data_str.length(); i++)
	{
        c = json_post_data_str[i];
        if (c != ' ' && c != '\n' && c != '\r')
			post_data += c;
	}

	/// API call
	string json_data_raw =
		http_post(URL_API_CREATE_TRANSACTION, post_data);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_createTransaction_obj obj;

	/// Fill API object fields
	if (json_data.isMember("success") && json_data["success"].isObject())
	{
		obj.order_id = json_data["success"]["orderId"].asString();
		obj.coin_pair = json_data["success"]["pair"].asString();
		obj.address_in = json_data["success"]["deposit"].asString();
		obj.amount_in =
			stod(json_data["success"]["depositAmount"].asString());
		obj.address_out = json_data["success"]["withdrawal"].asString();
		obj.amount_out =
			stod(json_data["success"]["withdrawalAmount"].asString());
		obj.return_address = json_data["success"]["returnAddress"].asString();
		obj.expiration = json_data["success"]["expiration"].asUInt64();
		obj.rate_fixed =
			stod(json_data["success"]["quotedRate"].asString());
		obj.limit_max = json_data["success"]["maxLimit"].asDouble();
		obj.miner_fee =
			stod(json_data["success"]["minerFee"].asString());
		obj.api_public_key = json_data["success"]["apiPubKey"].asString();
		obj.secondary_address = json_data["success"]["sAddress"].asString();
		obj.error = json_data["success"]["error"].asString();
	}
	else obj.error = json_data["error"].asString();

	return obj;
}

/// Quote Send Exact Price (api_createTransaction, overloaded function):
/// ...This call also allows you to request a quoted price on the amount of a
/// transaction without a withdrawal address.
api_createTransaction_obj
api_createTransaction(double amount, string coin_pair)
{
	/// Manually write JSON POST data
	string post_data =
		"{\"amount\":" + to_string(amount) + ",\"pair\":\"" + coin_pair +
			"\"}";

	/// API call
	string json_data_raw =
		http_post(URL_API_CREATE_TRANSACTION, post_data);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_createTransaction_obj obj;

	/// Fill API object fields
	if (json_data.isMember("success") && json_data["success"].isObject())
	{
		obj.order_id = json_data["success"]["orderId"].asString();
		obj.coin_pair = json_data["success"]["pair"].asString();
		//obj.address_in = json_data["success"]["deposit"].asString();
		obj.amount_in =
			stod(json_data["success"]["depositAmount"].asString());
		//obj.address_out = json_data["success"]["withdrawal"].asString();
		obj.amount_out =
			stod(json_data["success"]["withdrawalAmount"].asString());
		//obj.return_address = json_data["success"]["returnAddress"].asString();
		obj.expiration = json_data["success"]["expiration"].asUInt64();
		obj.rate_fixed =
			stod(json_data["success"]["quotedRate"].asString());
		obj.limit_max = json_data["success"]["maxLimit"].asDouble();
		obj.miner_fee =
			stod(json_data["success"]["minerFee"].asString());
		//obj.api_public_key = json_data["success"]["apiPubKey"].asString();
		obj.error = json_data["success"]["error"].asString();
	}
	else obj.error = json_data["error"].asString();

	return obj;
}

/// Cancel Pending Transaction (api_cancelTransaction):
/// This call allows you to request for canceling a pending transaction by the
/// deposit address. If there is fund sent to the deposit address, this
/// pending transaction cannot be canceled.
api_cancelTransaction_obj api_cancelTransaction(string address_in)
{
	/// Simple enough to write the JSON post data manually
	string post_data = "{\"address\":\"" + address_in + "\"}";

	/// API call
	string json_data_raw = http_post(URL_API_CANCEL_TRANSACTION, post_data);

	/// Interpret and extract JSON data
	Json::Reader json_reader;
	Json::Value json_data;
	json_reader.parse(json_data_raw, json_data);

	api_cancelTransaction_obj obj;

	/// Fill API object fields
	obj.success = json_data["success"].asString();
	obj.error = json_data["error"].asString();

	return obj;
}
