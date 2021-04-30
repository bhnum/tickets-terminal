#pragma once

#include <string>
#include <vector>

#include "commons.h"
#include "trip.h"

using namespace std;

struct Ticket {
	int id;
	int tripid;
	string holderusername;
	int chairnumber;
	bool canceled = false;
};

// Load tickets list form tickets.txt.
void loadtickets();

// Books a ticket. The ticket.id field is not required and will be generated automaticely.
Error bookticket(Ticket ticket, string discountcode = "");

// Cancle ticket by id.
// This function returns 90% of the cost of the ticket to the ticket holder if it is
// being canceled one hour before the departure time, otherwise 50% is returened.
void cancelticket(int id);

Ticket &getticket(int id);

vector<Ticket> &gettickets();

vector<Ticket> getticketsof(string holderusername);

vector<Ticket> getticketsof(int tripid);
