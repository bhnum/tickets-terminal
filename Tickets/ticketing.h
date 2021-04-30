#pragma once

#include <string>
#include <vector>

#include "commons.h"
#include "trip.h"

using namespace std;

struct Ticket {
	int id; // A unique randomly genareted 8-digit number
	int tripid;
	int holderid;
	int seatnumber;
	int cost;
	bool canceled = false;
};

// Load tickets list form tickets.txt.
void loadtickets();

// Books a ticket. The ticket.id field is not required and will be generated automaticely.
Error bookticket(Ticket &ticket, string discountcode = "");

// Cancle ticket by id.
// This function returns 90% of the cost of the ticket to the ticket holder if it is
// being canceled one hour before the departure time, otherwise 50% is returened.
bool cancelticket(int id);

bool editticket(Ticket ticket);

Ticket getticket(int id);

vector<Ticket> &gettickets();

vector<Ticket> getticketsof(int holderid);

// This function does not return canceled tickets;
vector<Ticket> gettickets(int tripid);

vector<Ticket> getalltickets(int tripid);
