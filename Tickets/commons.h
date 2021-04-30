#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

enum Error {
	Success,
	EmptyField,
	ExistingUserName,
	NonExistentRecord,
	NonExistentDriver,
	NonExistentTrip,
	InvalidField,
	InvalidCardNumber,
	InvalidSenderCardNumber,
	InvalidReciverCardNumber,
	InvalidCardPin,
	InsufficientFunds,
	UnknownError,
};

static inline tm getnowtime()
{
	time_t t = time(0);
	tm now = tm();
	localtime_s(&now, &t);
	return now;
}

static inline string tmtostring(tm t)
{
	stringstream ss;
	ss << put_time(&t, "%Y/%m/%d %R");
	return ss.str();
}

// https://stackoverflow.com/questions/11213326/how-to-convert-a-string-variable-containing-time-to-time-t-type-in-c/25037057#25037057
static inline tm stringtotm(string s)
{
	stringstream ss(s);
	tm t = tm();
	ss >> get_time(&t, "%Y/%m/%d %R");
	return t;
}
