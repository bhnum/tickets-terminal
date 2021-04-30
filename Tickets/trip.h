#pragma once

#include <string>
#include <vector>

using namespace std;

enum Vehicle {
	Bus, Train, Airplane
};

struct Trip {
	int id;
	Vehicle vehicle;
	int origincity;
	int destinationcity;
	string driverusername;
	tm departuretime;
};

// Load trips list from trips.txt.
void loadtrips();

// Returns the generated id of the defined trip.
// Returns -1 when failed (e.g. driver does not exist).
int definetrip(Trip trip);

// Editable fields: vehicle, origincity, destinationcity, and driverusername.
// The time field is not modifiable. To modify time, use the postponetrip function.
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

Trip &gettrip(int id);

vector<Trip> &gettrips();

vector<Trip> gettrips(string driverusername);

vector<Trip> filtertripsby(vector<Trip> &trips, Vehicle vehicle);

vector<Trip> filtertripsfrom(vector<Trip> &trips, int origincity);

vector<Trip> filtertripsto(vector<Trip> &trips, int destinationcity);

vector<Trip> filtertripson(vector<Trip> &trips, tm date);
