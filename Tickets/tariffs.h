#pragma once

#include <string>
#include <vector>

#include "trip.h"

using namespace std;

struct UserDiscount {
	int userid;
	vector<int> discounts;
};

struct DiscountCode {
	string code;
	int discountprecentage;
};

struct CityInfo {
	int number;
	int x, y;
};

// Loads user discount settings from userdiscounts.txt and discount codes from discountcodes.txt
// and city coordinates from cities.txt.
// User discounts format in userdiscounts.txt:
//		userid	numberofdiscounts	discount1	discount2	...
// Discount codes format in discountcodes.txt:
//		code		discountprecentage
void loadtariffs();

// Returns distance in km.
double gettripdistance(int origincity, int destinationcity);

// Returns time in hours.
double gettriptime(int origincity, int destinationcity, Vehicle vehicle);

// This function creates a record in user discount setting with default values if no records
// are found for this username. For guest users pass userid 0.
int getcostofticket(int tripid, int userid = 0, string discountcode = "");

// This function creates a record in user discount setting with default values if no records
// are found for this username.
void adduserdiscount(int userid, int discountprecentage);

void consumeuserdiscount(int userid);

// Returns false if an existing discount is being added, otherwise true.
bool definediscountcode(string code, int discountprecentage);

// Returns false if a not existing discount is being edited, otherwise true.
bool editdiscountcode(string code, int discountprecentage);

// Returns false if discount code does not exist, otherwise true.
bool deletediscountcode(string code);

int getdiscountofcode(string code);

UserDiscount getuserdiscountsof(int userid);

vector<UserDiscount> &getuserdiscounts();

vector<DiscountCode> &getdiscountcodes();
