#include "trip.h"
#include "users.h"
#include "ticketing.h"
#include "accounts.h"
#include "vehicleinformation.h"

#include <iomanip>
#include <fstream>

vector <Trip> trips;

static int generateid()
{
	int max = 0;
	for (int i = 0; i < trips.size(); i++)
	{
		if (trips[i].id > max)
			max = trips[i].id;
	}
	return max + 1;
}

void savetrips()
{
	ofstream file;

	file.open("trips.txt",ios::trunc);

	for (int i = 0; i < trips.size(); i++)
	{
		Trip &t = trips[i];

		file << t.id<< ",";
		file <<(int) t.vehicle << ",";
		file << t.origincity << ",";
		file << t.destinationcity<< ",";
		file << t.driverid << ",";
		file << put_time(&t.departuretime, "%Y/%m/%d %R") << ",";
		file << t.canceled << endl;
	}
	file.close();
}

void loadtrips()
{
	ifstream file;

	file.open("trips.txt");
	Trip t;
	while (file >> t.id)
	{
		file.ignore();
		file >> (int &)t.vehicle;
		file.ignore();
		file >> t.origincity;
		file.ignore();
		file >> t.destinationcity;
		file.ignore();
		file >> t.driverid;
		file.ignore();
		file >> get_time(&t.departuretime, "%Y/%m/%d %R");
		file.ignore();
		file >> t.canceled;
		trips.push_back(t);
	}
	file.close();
}

int definetrip(Trip trip)
{
	trip.id = generateid();
	trips.push_back(trip);
	savetrips();
	return trip.id;
}

bool edittrip(Trip trip)
{
	for (int i = 0; i < trips.size(); i++)
	{
		if (trip.id == trips[i].id)
		{
			trips[i] = trip;
			savetrips();
			return true;
		}
	}
	return false;
}

bool postponetrip(int tripid, tm newtime)
{
	Trip trip = gettrip(tripid);
	vector<Ticket> tickets = gettickets(tripid);

	tm t2 = newtime, t1 = trip.departuretime;
	int diff = difftime(mktime(&t2), mktime(&t1));
	if (diff <= 0) return false;

	double returnrate = 0.05 * diff / 3600;
	for (int i = 0; i < tickets.size(); i++)
	{
		int returns = tickets[i].cost * returnrate;
		string recievernumber = getuser(tickets[i].holderid).cardnumber;
		string drivernumber = getuser(gettrip(tickets[i].tripid).driverid).cardnumber;
		performtransaction(drivernumber, recievernumber, returns * 9 / 10);
		performtransaction(getadmin().cardnumber, recievernumber, returns / 10);
	}
	saveaccounts();

	trip.departuretime = newtime;
	edittrip(trip);
	return true;
}

bool canceltrip(int id)
{
	Trip trip = gettrip(id);
	vector<Ticket> tickets = gettickets(id);

	if (trip.canceled) return false;

	for (int i = 0; i < tickets.size(); i++)
	{
		string recievernumber = getuser(tickets[i].holderid).cardnumber;
		string drivernumber = getuser(gettrip(tickets[i].tripid).driverid).cardnumber;
		performtransaction(drivernumber, recievernumber, tickets[i].cost * 9 / 10);
		performtransaction(getadmin().cardnumber, recievernumber, tickets[i].cost / 10);
	}
	saveaccounts();

	trip.canceled = true;
	edittrip(trip);
	return true;
}

Trip gettrip(int id)
{
	for (int i = 0; i < trips.size(); i++)
	{
		if (id == trips[i].id)
		{
			return trips[i];
		}
	}
	return { 0 };
}

vector<Trip> &gettrips()
{
	return trips;
}

vector<Trip> getactivetrips()
{
	vector <Trip> activetrips;
	for (int i = 0; i <trips.size(); i++)
	{
		if (!trips[i].canceled)
		{
			activetrips.push_back(trips[i]);
		}
	}
	return activetrips;
}

vector<Trip> gettrips(string driverusername)
{
	int driverid= getuser(driverusername).id;

	return gettrips(driverid);	
}

vector<Trip> gettrips(int driverid)
{
	vector <Trip> drivertrips;
	for (int i = 0; i <trips.size() ; i++)
	{
		if (trips[i].driverid==driverid)
		{
			drivertrips.push_back(trips[i]);
		}
	}
	return drivertrips;
}

vector<Trip> filtertripsby(vector<Trip> &trips, Vehicle vehicle)
{
	vector <Trip> filtered;
	for (int i = 0; i <trips.size(); i++)
	{
		if (trips[i].vehicle == vehicle)
		{
			filtered.push_back(trips[i]);
		}
	}
	return filtered;
}

vector<Trip> filtertripsfrom(vector<Trip> &trips, int origincity)
{
	vector <Trip> filtered;
	for (int i = 0; i <trips.size(); i++)
	{
		if (trips[i].origincity == origincity)
		{
			filtered.push_back(trips[i]);
		}
	}
	return filtered;
}

vector<Trip> filtertripsto(vector<Trip> &trips, int destinationcity)
{
	vector <Trip> filtered;
	for (int i = 0; i <trips.size(); i++)
	{
		if (trips[i].destinationcity == destinationcity)
		{
			filtered.push_back(trips[i]);
		}
	}
	return filtered;
}

// Filter by date not time.
vector<Trip> filtertripson(vector<Trip> &trips, tm date)
{
	vector <Trip> filtered;
	for (int i = 0; i <trips.size(); i++)
	{
		if (trips[i].departuretime.tm_year == date.tm_year && 
			trips[i].departuretime.tm_mon == date.tm_mon &&
			trips[i].departuretime.tm_mday == date.tm_mday)
		{
			filtered.push_back(trips[i]);
		}
	}
	return filtered;
}



