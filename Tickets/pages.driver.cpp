#include "pages.h"

void showdriverpage(int userid)
{
	newlayout("Welcome " + getuser(userid).username + "!");

	putbutton("managetrips", "Manage trips"); puttext("");

	putbutton("editinfo", "Edit your information"); puttext("");

	putbutton("ESCAPE", "Log out", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);

	while (true)
	{
		string cmd = waitforuserinput();
		if (cmd == "managetrips")
		{
			showdrivermanagetripspage(userid);
		}
		else if (cmd == "editinfo")
		{
			showeditinfopage(userid);
			getlayout().caption = "Welcome " + getuser(userid).username + "!";
		}
		else if (cmd == "ESCAPE")
			break;
	}

	closelayout();
}

void showdrivermanagetripspage(int driverid)
{
	int page = 0;

	newlayout("Manage trips");

	while (true)
	{
		vector<Trip> trips = gettrips(driverid);

		putbutton("addtrip", "Define new trip", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);

		puttext("\n\nTrips list:");

		bool isempty = true;
		for (int i = page * 5; i < trips.size() && i < (page + 1) * 5; i++)
		{
			isempty = false;

			putbutton(to_string(i), "Trip #" + to_string(trips[i].id));
			puttext(" by " + vehicletostring(trips[i].vehicle) + " from city " + to_string(trips[i].origincity)
				+ " to " + to_string(trips[i].destinationcity) + " at " + tmtostring(trips[i].departuretime));
		}

		if (isempty)
			puttext("\n\n\t\tList is empty!");

		puttext("\n");
		if (page > 0)
			putbutton("previous", "Previous page");
		if (page < ((int)trips.size() - 1) / 5)
			putbutton("next", "Next page");
		putbutton("ESCAPE", "Back");
		puttext("  Page " + to_string(page + 1) + " of " + to_string(((int)trips.size() - 1) / 5 + 1));

		string cmd = waitforuserinput();

		if (cmd == "addtrip")
		{
			showdriveredittrippage(driverid, 0);
		}
		else if (cmd == "previous")
			page--;
		else if (cmd == "next")
			page++;
		else if (cmd == "ESCAPE")
			break;
		else
		{
			int index = stoi(cmd);
			showdrivertripinfopage(driverid, trips[index].id);
		}
		clearlayout();
	}
	closelayout();
}

void showdrivertripinfopage(int driverid, int tripid)
{
	int page = 0;

	newlayout("Information of trip #" + to_string(tripid));

	while (true)
	{
		Trip trip = gettrip(tripid);
		vector<Ticket> tickets = getalltickets(tripid);

		puttextbox("origin", "Origin", to_string(trip.origincity), false, true);
		puttextbox("destination", "Destination", to_string(trip.destinationcity), false, true);
		puttextbox("time", "Departure time", tmtostring(trip.departuretime), false, true);
		puttextbox("vehicle", "Vehicle", vehicletostring(trip.vehicle), false, true);
		puttextbox("canceled", "Is canceled", trip.canceled ? "Yes" : "No", false, true);

		putbutton("edit", "Edit trip information");
		if (!trip.canceled)
		{
			putbutton("postpone", "Postpone trip");
			putbutton("cancel", "Cancel trip", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);
		}

		puttext("\n\nTickets list of this trip:");

		bool isempty = true;
		for (int i = page * 5; i < tickets.size() && i < (page + 1) * 5; i++)
		{
			isempty = false;
			putbutton(to_string(i), "Ticket #" + to_string(tickets[i].id));
			puttext(" of seat #" + to_string(tickets[i].seatnumber) + " for user " + getuser(tickets[i].holderid).username);
			if (tickets[i].canceled) puttext(" (Canceled)", BackColorWhite | ForeColorRed);
		}

		if (isempty)
			puttext("\n\n\t\tList is empty!");

		puttext("\n");
		if (page > 0)
			putbutton("previous", "Previous page");
		if (page < ((int)tickets.size() - 1) / 5)
			putbutton("next", "Next page");
		putbutton("ESCAPE", "Back");
		puttext("  Page " + to_string(page + 1) + " of " + to_string(((int)tickets.size() - 1) / 5 + 1));


		string cmd = waitforuserinput();
		
		if (cmd == "edit")
		{
			showdriveredittrippage(driverid, tripid);
		}
		else if (cmd == "postpone")
		{
			newlayout("Postpone trip #" + to_string(tripid));
			puttextbox("", "Original time", tmtostring(trip.departuretime), false, true);
			puttextbox("time", "Postpone to", tmtostring(trip.departuretime));
			putbutton("postpone", "Postpone");
			putbutton("ESCAPE", "Back");
			puttext("errors", "");
			while (waitforuserinput() == "postpone")
			{
				tm newtime = stringtotm(getelement("time").text);
				if (!postponetrip(tripid, newtime))
					getelement("errors").text = "\n\nInvalid input.";
				else break;
			}
			closelayout();
		}
		else if (cmd == "cancel")
		{
			canceltrip(tripid);
		}
		else if (cmd == "previous")
			page--;
		else if (cmd == "next")
			page++;
		else if (cmd == "ESCAPE")
			break;
		else
		{
			int index = stoi(cmd);
			showdriverticketinfopage(tickets[index].id);
		}

		clearlayout();
	}

	closelayout();
}

void showdriveredittrippage(int driverid, int tripid)
{
	Trip trip = gettrip(tripid);

	newlayout(tripid == 0 ? "Define new trip" : "Edit trip #" + to_string(tripid));

	puttextbox("origin", "Origin", to_string(trip.origincity));
	puttextbox("destination", "Destination", to_string(trip.destinationcity));
	puttextbox("time", "Departure time (y/m/d h:m)", tripid == 0 ? "" : tmtostring(trip.departuretime));
	puttextbox("vehicle", "Vehicle (Bus/Train/Airplane)", vehicletostring(trip.vehicle));

	putbutton("apply", tripid == 0 ? "Add trip" : "Apply");
	putbutton("ESCAPE", "Back");

	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "apply")
		{
			try
			{
				Trip newtrip = {
					tripid,
					stringtovehicle(getelement("vehicle").text),
					stoi(getelement("origin").text),
					stoi(getelement("destination").text),
					driverid,
					stringtotm(getelement("time").text),
					trip.canceled,
				};
				if (tripid == 0)
					definetrip(newtrip);
				else
					edittrip(newtrip);
				break;
			}
			catch (exception)
			{
				getelement("errors").text = "\n\nInvalid input!";
			}
		}
		else if (cmd == "ESCAPE")
			break;
	}
	closelayout();
}

void showdriverticketinfopage(int ticketid)
{
	Ticket ticket = getticket(ticketid);
	Trip trip = gettrip(ticket.tripid);

	newlayout("Ticket #" + to_string(ticketid));

	puttextbox("", "Trip information", vehicletostring(trip.vehicle)
		+ " from city " + to_string(trip.origincity)
		+ " to " + to_string(trip.destinationcity)
		+ " at " + tmtostring(trip.departuretime), false, true);
	puttextbox("", "Seat number", to_string(ticket.seatnumber), false, true);
	puttextbox("", "Cost", to_string(ticket.cost), false, true);
	puttextbox("", "Tracking code", to_string(ticket.id), false, true);
	puttextbox("", "Is canceled", ticket.canceled ? "Yes" : "No", false, true);
	putbutton("holder", "Holder user #" + to_string(ticket.holderid)); puttext("");
	putbutton("ESCAPE", "Back");

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "holder")
			showdriveruserinfopage(ticket.holderid);
		else if (cmd == "ESCAPE")
			break;
	}
	closelayout();
}

void showdriveruserinfopage(int userid)
{
	newlayout("Information of user #" + to_string(userid));

	User user = getuser(userid);

	puttextbox("name", "Name", user.name, false, true);
	puttextbox("surname", "Surname", user.surname, false, true);
	puttextbox("phonenumber", "Phone number", user.phonenumber, false, true);
	if (user.refererid != 0)

	putbutton("ESCAPE", "OK");


	waitforuserinput();

	closelayout();
}
