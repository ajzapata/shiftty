#define SHIFTTY_INCLUDED /// TMP UNFINISHED; DONT INCLUDE THIS YET
#ifndef SHIFTTY_INCLUDED
#define SHIFTTY_INCLUDED

#include "shapeshift-api.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cinttypes>

/// Useful:
/// QR API: https://api.qrserver.com/v1/create-qr-code/?size=150x150&data=Example
/// - Website: http://goqr.me/api/

const bool TX_FAIL_EXEC_ON_PRICE_CHANGE = false;

/// Transaction class: Fixed-price Shapeshift transaction
/// Transaction objects hold all data necessary to create, execute, check,
/// reference, and cancel a transaction.
class Transaction
{
public:
	Transaction();
	Transaction(api_transactionStatus_obj obj);

private:
	// raw data; not the api objects

	/// Coin Pair
	string coin_pair;
	string coin_in;
	string coin_out;

	/// Coin Quantities
	double amount_in;
	double amount_out;

	/// Addresses
	string address_in;
	string address_out;
};

/// Order class: Bilateral execution of two transactions.
/// Output is a single coin amount, ideally larger than the initial quantity.
class Order
{
public:
	Order();
	Order(Transaction old_tx, Transaction new_tx);

private:
	// Transaction obj.s
};

#endif // SHIFTTY_INCLUDED
