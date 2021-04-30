#pragma once

#include <string>
#include <vector>

using namespace std;

enum Vehicle {
	Bus, Train, Airplane
};

struct Trip {
	int id; // Starts from 1
	Vehicle vehicle;
	int origincity;
	int destinationcity;
	int driverid;
	tm departuretime = tm();
	bool canceled = false;
};

// Load trips list from trips.txt.
void loadtrips();

// Returns the generated id of the defined trip.
// Returns -1 when failed (e.g. driver does not exist).
int definetrip(Trip trip);

// Returns true if succeeded, otherwise false (e.g. trip or driver does not exist).
bool edittrip(Trip trip);

// Returns true if succeeded, otherwise false (e.g. trip does not exist or
// newtime is before the original time).
// This function should also return 5% of the cost of the trip ticket for each hour
// of delaying to the holders of the tickets.
bool postponetrip(int tripid, tm newtime);

// Returns true if succeeded, otherwise false (e.g. trip not found).
// This function should also return the cost of the trip ticket to the ticket holders.
bool canceltrip(int id);

Trip gettrip(int id);

vector<int> getemptyseats(int tripid);

vector<Trip> &gettrips();

vector<Trip> getactivetrips();

vector<Trip> gettrips(string driverusername);

vector<Trip> gettrips(int driverid);

vector<Trip> filtertripsby(vector<Trip> &trips, Vehicle vehicle);

vector<Trip> filtertripsfrom(vector<Trip> &trips, int origincity);

vector<Trip> filtertripsto(vector<Trip> &trips, int destinationcity);

// Filter by date not time.
vector<Trip> filtertripson(vector<Trip> &trips, tm date);
