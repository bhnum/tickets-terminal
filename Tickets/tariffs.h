#pragma once

#include <string>
#include <vector>

using namespace std;

struct DiscountCode {
	string code;
	int discountprecentage;
};

// Loads user discount settings from userdiscounts.txt and discount codes from discountcodes.txt.
// User discounts format in userdiscounts.txt:
//		username	numberofdiscounts	discount1	discount2	...
// Discount codes format in discountcodes.txt:
//		code		discountprecentage
void loaddiscounts();

// This function creates a record in user discount setting with default values if no records
// are found for this username. For guest users pass username "".
int getcostofticket(int tripid, string username = "", string discountcode = "");

// This function creates a record in user discount setting with default values if no records
// are found for this username.
void adduserdiscount(string username, int discountprecentage);

// Returns false if an existing discount is being added or discountprecentage is < 0 or > 100, otherwise true.
bool definediscountcode(string code, int discountprecentage);

// Returns false if an existing discount is being renamed to or discountprecentage is < 0 or > 100, otherwise true.
bool editdiscountcode(string code, int discountprecentage);

// Returns false if discount code does not exist, otherwise true.
bool deletediscountcode(string code);

vector<DiscountCode> &getdiscountcodes();
