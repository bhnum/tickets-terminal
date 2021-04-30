#pragma once

#include <string>
#include <vector>

#include "commons.h"

using namespace std;

struct Transaction {
	int id;
	int sendernumber;
	int recievernumber;
	int amount;
	tm time;
};

// Load accounts and transactions from accounts.txt and transactions.txt.
void loadaccounts();

Error validatepin(int number, int pin);

Error getcardbalance(int number, int &balance);

Error performtransaction(Transaction transaction);

Transaction &gettransaction(int id);

vector<Transaction> &gettransactions();

vector<Transaction> gettransactionsfrom(int number);

vector<Transaction> gettransactionsto(int number);
