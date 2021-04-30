#pragma once

#include <string>
#include <vector>

#include "commons.h"

using namespace std;

void saveaccounts();

// Load accounts and transactions from accounts.txt and transactions.txt.
void loadaccounts();

Error validatepin(string number, string pin);

// Returns the balance if succeeded, -1 otherwise.
int getcardbalance(string number);

Error performtransaction(string sendernumber, string recievernumber, int amount, bool force = false);
