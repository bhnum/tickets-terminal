#pragma once

#include <string>
#include <vector>
#include <ctime>

#include "layout.h"
#include "users.h"
#include "accounts.h"
#include "ticketing.h"
#include "vehicleinformation.h"
#include "waitinglist.h"
#include "tariffs.h"

using namespace std;

// This macro uses 2 local variables: error of type bool, and errorstext of type string which is displayed to the user.
// The parameter is the name of the element to be checked.
#define validatefield(name)	if (getelement(name).text == "") { \
								error = true; \
								errorstext += "\n" + getelement(name).title + " field is required!"; \
							} \
							else if (getelement(name).text.find(",") != string::npos) { \
								error = true; \
								errorstext += "\n" + getelement(name).title + " field should not contain commas."; \
							}

void showmainpage();

void showloginpage();

void showregisterpage();

void showeditinfopage(int userid);

// User
void showuserpage(int userid);

void showselecttrippage(int userid);

bool showbookticketpage(int userid, int tripid, int seatnumber);

void showbookedticketpage(int ticketid);

void showtrackticketpage(int userid);

void showinwaitinglists(int userid);

// Driver
void showdriverpage(int userid);

void showdrivermanagetripspage(int driverid);

void showdrivertripinfopage(int driverid, int tripid);

void showdriveredittrippage(int driverid, int tripid);

void showdriverticketinfopage(int ticketid);

void showdriveruserinfopage(int userid);

// Admin
void showadminpage(int userid);

void showeditvehicleinfopage();

void showmanageuserspage();

void showuserinfopage(int userid);

void showedituserpage(int userid = 0);

void showeditdriverpage(int userid = 0);

void showmanagetripspage();

void showtripinfopage(int tripid);

void showedittrippage(int tripid);

void showticketinfopage(int ticketid);

void showmanagediscountcodespage();

static inline string vehicletostring(Vehicle v)
{
	switch (v)
	{
	case Bus:
		return "Bus";
	case Train:
		return "Train";
	case Airplane:
		return "Airplane";
	}
}

static inline Vehicle stringtovehicle(string s)
{
	if (s == "Bus") return Bus;
	if (s == "Train") return Train;
	if (s == "Airplane") return Airplane;
	throw invalid_argument("");
}


vector<int> getemptyseats(int tripid);