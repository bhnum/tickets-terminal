#include "pages.h"


void showuserpage(int userid)
{
	newlayout("Welcome " + getuser(userid).username + "!");

	putbutton("bookticket", "Book a ticket"); puttext("");
	putbutton("trackticket", "See and track your tickets"); puttext("");
	putbutton("managewaitinglists", "Manage your waiting lists"); puttext("");
	putbutton("editinfo", "Edit your information"); puttext("");

	putbutton("ESCAPE", "Log out", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);

	while (true)
	{
		string cmd = waitforuserinput();
		if (cmd == "bookticket")
		{
			showselecttrippage(userid);
		}
		else if (cmd == "trackticket")
		{
			showtrackticketpage(userid);
		}
		else if (cmd == "managewaitinglists")
		{
			showinwaitinglists(userid);
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

void showselecttrippage(int userid)
{
	tm today = getnowtime();

	newlayout("Select trip");

	puttextbox("vehicle", "Vehicle (Bus/Train/Airplane)", "");
	puttextbox("origin", "Origin city", "");
	puttextbox("destination", "Destination city", "");
	puttextbox("date", "Departure date (y/m/d)",
		to_string(today.tm_year + 1900) + "/" + to_string(today.tm_mon + 1) + "/" + to_string(today.tm_mday));

	putbutton("search", "Search", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
	putbutton("ESCAPE", "Back");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "search")
		{
			try
			{
				vector<Trip> trips = getactivetrips();
				trips = filtertripsby(trips, stringtovehicle(getelement("vehicle").text));
				trips = filtertripsfrom(trips, stoi(getelement("origin").text));
				trips = filtertripsto(trips, stoi(getelement("destination").text));
				trips = filtertripson(trips, stringtotm(getelement("date").text + " 00:00"));
				int page = 0;

				newlayout("Trips by " + getelement("vehicle").text + " from " + getelement("origin").text
					+ " to " + getelement("destination").text + " on " + getelement("date").text);

				while (true)
				{
					puttext("\n\nSelect trip");

					bool isempty = true;
					for (int i = page * 5; i < trips.size() && i < (page + 1) * 5; i++)
					{
						isempty = false;

						putbutton(to_string(i), vehicletostring(trips[i].vehicle) + " from city " + to_string(trips[i].origincity)
							+ " to " + to_string(trips[i].destinationcity) + " at " + tmtostring(trips[i].departuretime));
						puttext("", " Cost: " + to_string(getcostofticket(trips[i].id, userid)));
					}

					if (isempty)
						puttext("\n\n\t\tNo trips found!");

					puttext("\n");
					if (page > 0)
						putbutton("previous", "Previous page");
					if (page < ((int)trips.size() - 1) / 5)
						putbutton("next", "Next page");
					putbutton("ESCAPE", "Back");
					puttext("  Page " + to_string(page + 1) + " of " + to_string(((int)trips.size() - 1) / 5 + 1));
					string cmd = waitforuserinput();

					if (cmd == "previous")
						page--;
					else if (cmd == "next")
						page++;
					else if (cmd == "ESCAPE")
						break;
					else
					{
						int index = stoi(cmd);
						int seats;
						switch (trips[index].vehicle)
						{
						case Bus:
							seats = getvehicleinformation().buscapacity;
							break;
						case Train:
							seats = getvehicleinformation().traincapacity;
							break;
						case Airplane:
							seats = getvehicleinformation().airplanecapacity;
							break;
						}
						vector<int> emptyseats = getemptyseats(trips[index].id);
						vector<bool> seattaken(seats, true);
						for (int i = 0; i < emptyseats.size(); i++)
							seattaken[emptyseats[i] - 1] = false;

						// Unit of ETA: 30 minutes
						int eta = round(gettriptime(trips[index].origincity, trips[index].destinationcity, trips[index].vehicle) * 2);

						newlayout(getelement(cmd).text);

						puttextbox("", "Vehicle", vehicletostring(trips[index].vehicle), false, true);
						puttextbox("", "Origin city", to_string(trips[index].origincity), false, true);
						puttextbox("", "Destination city", to_string(trips[index].destinationcity), false, true);
						puttextbox("", "Departure time", tmtostring(trips[index].departuretime), false, true);
						puttextbox("", "Cost", to_string(getcostofticket(trips[index].id, userid)), false, true);
						puttextbox("", "Distance", to_string((int)gettripdistance(trips[index].origincity, trips[index].destinationcity)) + " km", false, true);
						puttextbox("", "Estimated time of arrival", to_string(eta / 2) + ":" + (eta % 2 == 0 ? "00" : "30"), false, true);
						
						if (emptyseats.size() == 0)
						{
							puttext("\n\nThis vehicle is full. All seats are taken!");
							if (userid != 0)
							{
								if (!isuserinwaitinglist(userid, trips[index].id))
									putbutton("addtowaitinglist", "Add me to waiting list");
								else
									puttext("\n\nYou are in the waiting list.");
							}
							else
							{
								puttext("\n\nLog in to be able to be added to waiting list");
							}
							putbutton("ESCAPE", "Back");
							puttext("errors", "", BackColorWhite | ForeColorRed);
						}
						else if (trips[index].vehicle == Bus)
						{
							puttext("\n\nSelect seat");
							for (int i = 0; i < seats; i += 3)
								putbutton(seattaken[i] ? "" : to_string(i + 1), to_string(i + 1),
									seattaken[i] ? (BackColorRed | ForeColorWhite) : (BackColorGreen | ForeColorWhite),
									seattaken[i] ? (BackColorLightRed | ForeColorWhite) : (BackColorLightGreen | ForeColorWhite));
							puttext("");
							for (int i = 1; i < seats; i += 3)
								putbutton(seattaken[i] ? "" : to_string(i + 1), to_string(i + 1),
									seattaken[i] ? (BackColorRed | ForeColorWhite) : (BackColorGreen | ForeColorWhite),
									seattaken[i] ? (BackColorLightRed | ForeColorWhite) : (BackColorLightGreen | ForeColorWhite));
							puttext("\n");
							for (int i = 2; i < seats; i += 3)
								putbutton(seattaken[i] ? "" : to_string(i + 1), to_string(i + 1),
									seattaken[i] ? (BackColorRed | ForeColorWhite) : (BackColorGreen | ForeColorWhite),
									seattaken[i] ? (BackColorLightRed | ForeColorWhite) : (BackColorLightGreen | ForeColorWhite));
							puttext("");
							putbutton("ESCAPE", "Back");
						}
						else
						{
							struct Range {
								int start, end;
							};
							vector<Range> ranges;
							for (int i = 0; i < emptyseats.size(); i++)
							{
								if (ranges.size() == 0 || ranges.back().end != emptyseats[i] - 1)
									ranges.push_back({ emptyseats[i], emptyseats[i] });
								else
									ranges.back().end++;
							}
							string availableseats;
							for (int i = 0; i < ranges.size(); i++)
							{
								if (ranges[i].start == ranges[i].end)
									availableseats += ", " + to_string(ranges[i].start);
								else
									availableseats += ", " + to_string(ranges[i].start) + " - " + to_string(ranges[i].end);
							}
							availableseats = availableseats.substr(2);
							puttext("\n\nAvailable seats: " + availableseats);
							puttextbox("seat", "Enter seat number", "");
							putbutton("select", "Select seat", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
							puttext("errors", "", BackColorWhite | ForeColorRed);
							putbutton("ESCAPE", "Back");

							/*for (int i = 0; i < seats / 6 + 1; i++)
							{
								for (int j = 0; j < 6 && i * 6 + j < seats; j++)
								{
									putbutton(seatinfo[i] ? "" : to_string(i * 6 + j + 1), to_string(i * 6 + j + 1),
										seatinfo[i * 6 + j] ? (BackColorRed | ForeColorWhite) : (BackColorGreen | ForeColorWhite),
										seatinfo[i * 6 + j] ? (BackColorLightRed | ForeColorWhite) : (BackColorLightGreen | ForeColorWhite));
								}
								puttext("");
							}*/
						}

						while (true)
						{
							clearallscreen();
							string cmd = waitforuserinput();

							if (cmd == "addtowaitinglist")
							{
								if (!addusertowaitinglist(userid, trips[index].id))
									getelement("errors").text = "\n\nThe waiting list for this trip is full!";
								else
								{
									newlayout("Message");
									puttext("\n\n\t\t\t\tYour have been added to the waiting list!");
									putbutton("", "OK", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
									waitforuserinput();
									closelayout();

									closelayout();
									closelayout();
									closelayout();
									return;
								}
							}
							else if (cmd == "select")
							{
								int seatnumber = stoi(getelement("seat").text);
								if (seattaken[seatnumber - 1])
									getelement("errors").text = "  This seat is taken!";
								else if (showbookticketpage(userid, trips[index].id, seatnumber))
								{
									closelayout();
									closelayout();
									closelayout();
									return;
								}
							}
							else if (cmd == "ESCAPE")
								break;
							else if (cmd != "")
							{
								int seatnumber = stoi(cmd);
								if (showbookticketpage(userid, trips[index].id, seatnumber))
								{
									closelayout();
									closelayout();
									closelayout();
									return;
								}
							}
						}
						closelayout();
					}
					clearlayout();
				}
				closelayout();
			}
			catch (exception)
			{
				getelement("errors").text = "\n\nInvalid input! Please try again.";
			}
		}
		else if (cmd == "ESCAPE")
			break;
	}
	closelayout();
}

bool showbookticketpage(int userid, int tripid, int seatnumber)
{
	Trip trip = gettrip(tripid);
	User user = getuser(userid);

	newlayout("Book ticket");

	puttextbox("", "Ticket information", vehicletostring(trip.vehicle)
		+ " from city " + to_string(trip.origincity)
		+ " to " + to_string(trip.destinationcity)
		+ " at " + tmtostring(trip.departuretime)
		+ " seat number " + to_string(seatnumber), false, true);
	puttext("\n\nYour information:");
	puttextbox("name", "Name", user.name, false, userid != 0);
	puttextbox("surname", "Surname", user.surname, false, userid != 0);
	puttextbox("phonenumber", "Phone number", user.phonenumber, false, userid != 0);
	puttextbox("cardnumber", "Card number", user.cardnumber, false, userid != 0);
	puttextbox("cardpin", "Card PIN", user.cardpin, false, userid != 0);
	puttext("\n");
	puttextbox("discountcode", "Discount code", "");
	putbutton("checkcode", "Check discount code");
	puttextbox("cost", "Cost", to_string(getcostofticket(tripid, userid)), false, true);
	putbutton("book", "Book this ticket");
	putbutton("ESCAPE", "Back");
	puttext("\n");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();
		string &errorstext = getelement("errors").text;
		errorstext = "";

		if (cmd == "book")
		{
			if (userid == 0)
			{
				bool error = false;
				validatefield("name");
				validatefield("surname");
				validatefield("phonenumber");
				validatefield("cardnumber");
				validatefield("cardpin");

				if (!error)
				{
					string discountcode = getelement("discountcode").text;
					if (discountcode != "" && getdiscountofcode(discountcode) == -1)
					{
						error = true;
						errorstext += "\nDiscount code does not exist!";
					}
				}

				if (!error)
				{
					User user = {
						0, GuestUser, "", "",
						getelement("cardnumber").text,
						getelement("cardpin").text,
						getelement("name").text,
						getelement("surname").text,
						getelement("phonenumber").text,
						0
					};
					Error e = registeruser(user);
					if (e == Success)
					{
						userid = user.id;

						Ticket ticket = {
							0, tripid, userid, seatnumber,
							0, false
						};
						e = bookticket(ticket, getelement("discountcode").text);

						if (e == Success)
						{
							closelayout();
							showbookedticketpage(ticket.id);
							return true;
						}
						else if (e == InsufficientFunds)
							errorstext += "\nNot enough funds in the provided account!";
						else
							errorstext += "\nUnknown error!!!";
					}
					else if (e == InvalidCardNumber)
						errorstext += "\nThe provided card number is invalid!";
					else if (e == InvalidCardPin)
						errorstext += "\nThe provided card PIN is invalid!";
					else
						errorstext += "\nUnknown error!!!";
				}
			}
			else
			{
				Ticket ticket = {
					0, tripid, userid, seatnumber,
					0, false
				};
				Error e = bookticket(ticket, getelement("discountcode").text);

				if (e == Success)
				{
					closelayout();
					showbookedticketpage(ticket.id);
					return true;
				}
				else if (e == InsufficientFunds)
					errorstext += "\nNot enough funds in the provided account!";
				else if (e == InvalidCardNumber)
					errorstext += "\nThe provided card number is invalid!";
				else if (e == InvalidCardPin)
					errorstext += "\nThe provided card PIN is invalid!";
				else
					errorstext += "\nUnknown error!!!";
			}

		}
		else if (cmd == "checkcode")
		{
			string discountcode = getelement("discountcode").text;
			if (getdiscountofcode(discountcode) == -1)
			{
				errorstext += "\nDiscount code does not exist!";
			}
			else
			{
				getelement("cost").text = to_string(getcostofticket(tripid, userid, discountcode))
					+ " (" + to_string(getdiscountofcode(discountcode)) + "% discount)";
			}
		}
		else if (cmd == "ESCAPE")
			break;
	}

	closelayout();
	return false;
}

void showbookedticketpage(int ticketid)
{
	Ticket ticket = getticket(ticketid);
	Trip trip = gettrip(ticket.tripid);

	newlayout("Ticket information");

	puttextbox("", "Ticket information", vehicletostring(trip.vehicle)
		+ " from city " + to_string(trip.origincity)
		+ " to " + to_string(trip.destinationcity)
		+ " at " + tmtostring(trip.departuretime), false, true);
	puttextbox("", "Seat number", to_string(ticket.seatnumber), false, true);
	puttextbox("", "Cost", to_string(ticket.cost), false, true);
	puttextbox("", "Tracking code", to_string(ticket.id), false, true);
	puttextbox("", "Is canceled", ticket.canceled ? "Yes" : "No", false, true);

	if (!ticket.canceled)
		putbutton("cancel", "Cancel ticket", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);
	putbutton("ESCAPE", "Back");

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "cancel")
		{
			cancelticket(ticketid);
			break;
		}
		else if (cmd == "ESCAPE")
			break;
	}
	closelayout();
}

void showtrackticketpage(int userid)
{
	int page = 0;
	string error;

	newlayout(userid == 0 ? "Track ticket" : "Track and see your booked tickets");

	while (true)
	{
		vector<Ticket> tickets = getticketsof(userid);

		puttextbox("id", "Enter tracking code", "");
		putbutton("track", "Search");
		puttext("error", error, BackColorWhite | ForeColorRed);

		if (userid != 0)
		{
			puttext("\n\nYour saved tickets:");

			bool isempty = true;
			for (int i = page * 5; i < tickets.size() && i < (page + 1) * 5; i++)
			{
				isempty = false;
				Trip trip = gettrip(tickets[i].tripid);
				putbutton(to_string(i), vehicletostring(trip.vehicle)
					+ " from city " + to_string(trip.origincity)
					+ " to " + to_string(trip.destinationcity)
					+ " at " + tmtostring(trip.departuretime));
				puttext("");
				if (tickets[i].canceled)
					puttext(" Canceled", BackColorWhite | ForeColorRed);
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
		}
		else
			putbutton("ESCAPE", "Back");

		string cmd = waitforuserinput();

		if (cmd == "track")
		{
			try
			{
				int id = stoi(getelement("id").text);
				if (getticket(id).id != 0)
					showbookedticketpage(id);
				else
					error = "  Code does not exist!";
			}
			catch (exception)
			{
				error = "  Code does not exist!";
			}
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
			showbookedticketpage(tickets[index].id);
		}
		clearlayout();
	}

	closelayout();
}

void showinwaitinglists(int userid)
{
	int page = 0;

	newlayout("Manage trips");

	while (true)
	{
		vector<int> tripids = gettripsinwaitinglist(userid);
		vector<Trip> trips;
		for (int i = 0; i < tripids.size(); i++)
			trips.push_back(gettrip(tripids[i]));

		puttext("\n\nTrips in waiting list:");

		bool isempty = true;
		for (int i = page * 5; i < trips.size() && i < (page + 1) * 5; i++)
		{
			isempty = false;

			puttext(vehicletostring(trips[i].vehicle) + " from city " + to_string(trips[i].origincity)
				+ " to " + to_string(trips[i].destinationcity) + " at " + tmtostring(trips[i].departuretime));
			putbutton(to_string(i), "Remove me from waiting list", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);
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

		if (cmd == "previous")
			page--;
		else if (cmd == "next")
			page++;
		else if (cmd == "ESCAPE")
			break;
		else
		{
			int index = stoi(cmd);
			removeuserfromwaitinglist(userid, trips[index].id);
		}
		clearlayout();
	}
	closelayout();
}
