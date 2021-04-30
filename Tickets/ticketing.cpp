#include "ticketing.h"
#include "tariffs.h"
#include "users.h"
#include "accounts.h"
#include "waitinglist.h"
#include <fstream>
#include <random>
#include <ctime>

using namespace std;

vector<Ticket> tickets;

static int generateid()
{
	int id;
	do {
		id = rand() % (1000000 - 100000) + 100000;
	} while (getticket(id).id != 0);
	return id;
}

void savetickets()
{
	ofstream file;
	file.open("tickets.txt");
	for (int i = 0; i < tickets.size(); i++) {
		Ticket &Ti = tickets[i];
		file << Ti.id << ",";

		file << Ti.tripid << ",";
		file << Ti.holderid << ",";
		file << Ti.seatnumber << ",";
		file << Ti.cost << ",";
		file << Ti.canceled << endl;
	}
	file.close();
}


void loadtickets()
{
	srand(time(0));
	ifstream file;
	file.open("tickets.txt");
	Ticket Ti;
	while (file >> Ti.id)
	{
		file.ignore();
		file >> Ti.tripid;
		file.ignore();
		file >> Ti.holderid;
		file.ignore();
		file >> Ti.seatnumber;
		file.ignore();
		file >> Ti.cost;
		file.ignore();
		file >> Ti.canceled;
		
		tickets.push_back(Ti);
	}
	file.close();
}

Error bookticket(Ticket &ticket, string discountcode)
{
	int cost = getcostofticket(ticket.tripid,ticket.holderid,discountcode );
	User user = getuser(ticket.holderid);

	Error e = validatepin(user.cardnumber, user.cardpin);
	if (e != Success)
		return e;
	
	if (cost > getcardbalance(user.cardnumber))
	{
		return InsufficientFunds;
	}

	string drivernumber = getuser(gettrip(ticket.tripid).driverid).cardnumber;
	performtransaction(user.cardnumber, drivernumber, cost * 9 / 10);
	performtransaction(user.cardnumber, getadmin().cardnumber, cost / 10);
	saveaccounts();
	
	if (discountcode == "")
		consumeuserdiscount(ticket.holderid);

	ticket.id = generateid();
	ticket.cost = cost;
	tickets.push_back(ticket);
	savetickets();

	vector<Ticket> tickets = getticketsof(ticket.holderid);
	if (tickets.size() % 4 == 0)
		adduserdiscount(ticket.holderid, 40);

	return Success;
}

bool cancelticket(int id)
{
	Ticket &ticket = getticket(id);
	User user = getuser(ticket.holderid);
	Trip trip = gettrip(ticket.tripid);

	if (ticket.canceled) return false;

	int now = time(0);
	int time = mktime(&trip.departuretime);
	int diff = difftime(time, now);

	int returnprecentage;
	if (diff >= 3600)
		returnprecentage = 90;
	else
		returnprecentage = 50;

	string drivernumber = getuser(gettrip(ticket.tripid).driverid).cardnumber;
	performtransaction(drivernumber, user.cardnumber, ticket.cost * 9 / 10 * returnprecentage / 100);
	performtransaction(getadmin().cardnumber, user.cardnumber, ticket.cost / 10 * returnprecentage / 100);
	ticket.canceled = true;
	editticket(ticket);

	updatewaitinglists();
	saveaccounts();

	return true;
}

bool editticket(Ticket ticket)
{
	for (int i = 0; i < tickets.size(); i++)
	{
		if (tickets[i].id == ticket.id)
		{
			tickets[i] = ticket;
			savetickets();
			return true;
		}
	}
	return false;
}

Ticket getticket(int id)
{
	for (int i = 0; i < tickets.size(); i++)
	{
		if (tickets[i].id == id)
			return tickets[i];
	}
	return { 0 };
}

vector<Ticket>& gettickets()
{
	return tickets;
}

vector<Ticket> getticketsof(int holderid)
{
	vector<Ticket> ticketsof;
	for (int i = 0; i < tickets.size(); i++)
	{
		if (tickets[i].holderid == holderid)
			ticketsof.push_back(tickets[i]);
	}
	return ticketsof;
}

vector<Ticket> gettickets(int tripid)
{
	vector<Ticket> ticketsof;
	for (int i = 0; i < tickets.size(); i++)
	{
		if (tickets[i].tripid == tripid && !tickets[i].canceled)
			ticketsof.push_back(tickets[i]);
	}
	return ticketsof;
}

vector<Ticket> getalltickets(int tripid)
{
	vector<Ticket> ticketsof;
	for (int i = 0; i < tickets.size(); i++)
	{
		if (tickets[i].tripid == tripid)
			ticketsof.push_back(tickets[i]);
	}
	return ticketsof;
}
