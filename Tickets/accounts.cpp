#include "accounts.h"
#include <fstream>

struct Account
{
	string number;
	string pin;
	int balance;
};

vector<Account> accounts;

void saveaccounts()
{
	ofstream file;
	file.open("accounts.txt", ios::trunc);

	for (int i = 0; i < accounts.size(); i++)
	{
		file << accounts[i].number << "\t";
		file << accounts[i].pin    << "\t";
		file << accounts[i].balance << endl;
		
	}
	file.close();
}
void loadaccounts() {

	ifstream file;
	file.open("accounts.txt");
	
	Account a;
	while (getline(file, a.number, '\t'))
	{
		getline(file, a.pin, '\t');
		file >> a.balance;
		accounts.push_back(a);

		getline(file, a.number); // remove new line character.
	}
	file.close();
}

Error validatepin(string number, string pin)
{
	for (int i = 0; i < accounts.size(); i++)
	{
		if (accounts[i].number == number)
		{
			if (accounts[i].pin == pin) {
				return Success;
			}
			return InvalidCardPin;
		}
	}
	return InvalidCardNumber;
}

int getcardbalance(string number)
{
	for (int i = 0; i < accounts.size(); i++)
	{
		if (accounts[i].number == number)
		{
			return accounts[i].balance;
		}
	}
	return -1;
}

Error performtransaction(string sendernumber, string recievernumber, int amount, bool force)
{
	for (int i = 0; i < accounts.size(); i++)
	{
		if (accounts[i].number == sendernumber)
		{
			if (force || accounts[i].balance >= amount)
			{
				for (int j = 0; j < accounts.size(); j++)
				{
					if (accounts[j].number == recievernumber)
					{
						accounts[i].balance -= amount;
						accounts[j].balance += amount;
						return Success;
					}
				}
				return InvalidReciverCardNumber;
			}
			return InsufficientFunds;
		}
	}
	return InvalidSenderCardNumber;
}