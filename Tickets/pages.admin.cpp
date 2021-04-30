#include "pages.h"

void showadminpage(int userid)
{
	User user = getuser(userid);
	if (getcardbalance(user.cardnumber) == -1)
	{
		newlayout("Enter card number");
		puttextbox("cardnumber", "Please enter your card number", "");
		putbutton("apply", "OK");
		puttext("errors", "", BackColorWhite | ForeColorRed);

		while (true)
		{
			string cmd = waitforuserinput();
			if (cmd == "apply")
			{
				string cardnumber = getelement("cardnumber").text;
				if (getcardbalance(cardnumber) == -1)
					getelement("error").text = "\n\nInvalid card number!";
				else
				{
					user.cardnumber = cardnumber;
					edituser(user);
					break;
				}
			}

			else if (cmd == "ESCAPE")
			{
				closelayout();
				return;
			}
		}
		closelayout();
	}

	newlayout("Welcome " + user.username + "!");

	putbutton("manageinfo", "Manage vehicle information"); puttext("");
	putbutton("manageusers", "Manage users"); puttext("");
	putbutton("managetrips", "Manage trips"); puttext("");
	putbutton("managediscountcodes", "Manage discount codes"); puttext("");

	putbutton("editinfo", "Edit your information"); puttext("");

	putbutton("ESCAPE", "Log out", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);

	while (true)
	{
		string cmd = waitforuserinput();
		if (cmd == "manageinfo")
		{
			showeditvehicleinfopage();
		}
		else if (cmd == "manageusers")
		{
			showmanageuserspage();
		}
		else if (cmd == "managetrips")
		{
			showmanagetripspage();
		}
		else if (cmd == "managediscountcodes")
		{
			showmanagediscountcodespage();
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

void showeditvehicleinfopage()
{
	VehicleInformation info = getvehicleinformation();

	newlayout("Edit vehicle information");
	
	puttextbox("buscostcoefficient", "Bus cost per km", to_string(info.buscostcoefficient));
	puttextbox("traincostcoefficient", "Train cost per km", to_string(info.traincostcoefficient));
	puttextbox("airplanecostcoefficient", "Airplane cost per km", to_string(info.airplanecostcoefficient));

	puttextbox("busspeed", "Bus speed km (km/h)", to_string(info.busspeed));
	puttextbox("trainspeed", "Train speed (km/h)", to_string(info.trainspeed));
	puttextbox("airplanespeed", "Airplane speed (km/h)", to_string(info.airplanespeed));

	puttextbox("buscapacity", "Bus passenger capacity", to_string(info.buscapacity));
	puttextbox("traincapacity", "Train passenger capacity", to_string(info.traincapacity));
	puttextbox("airplanecapacity", "Airplane passenger capacity", to_string(info.airplanecapacity));

	puttextbox("buswaitinglistcapacity", "Bus waiting list capacity", to_string(info.buswaitinglistcapacity));
	puttextbox("trainwaitinglistcapacity", "Trian waiting list capacity", to_string(info.trainwaitinglistcapacity));
	puttextbox("airplanewaitinglistcapacity", "Airplane waiting list capacity", to_string(info.buswaitinglistcapacity));

	putbutton("change", "Change info");
	putbutton("ESCAPE", "Back");

	puttext("errors", "");

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "change")
		{
			try
			{
				info = {
					stoi(getelement("buscostcoefficient").text),
					stoi(getelement("traincostcoefficient").text),
					stoi(getelement("airplanecostcoefficient").text),

					stoi(getelement("busspeed").text),
					stoi(getelement("trainspeed").text),
					stoi(getelement("airplanespeed").text),

					stoi(getelement("buscapacity").text),
					stoi(getelement("traincapacity").text),
					stoi(getelement("airplanecapacity").text),

					stoi(getelement("buswaitinglistcapacity").text),
					stoi(getelement("trainwaitinglistcapacity").text),
					stoi(getelement("airplanewaitinglistcapacity").text),
				};
				editvehicleinformation(info);
				break;
			}
			catch (exception)
			{
				getelement("errors").text = "\n\nInvalid input!";
			}
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
	}
	closelayout();
}

void showmanageuserspage()
{
	int page = 0;
	string error;

	newlayout("Manage users");

	while (true)
	{
		vector<User> users = getusers();

		putbutton("adduser", "Add new user", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
		putbutton("adddriver", "Add new driver", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);

		puttextbox("username", "Search by username", "");
		putbutton("search", "Search", BackColorBlue | ForeColorWhite, BackColorLightBlue | ForeColorWhite);
		puttext("error", error, BackColorWhite | ForeColorRed);

		puttext("\n\nUsers list:");

		bool isempty = true;
		for (int i = page * 5; i < users.size() && i < (page + 1) * 5; i++)
		{
			isempty = false;
			putbutton(to_string(i), "User #" + to_string(users[i].id));
			puttext("\t  " + usertypetostring(users[i].type) + "\t\t" + users[i].username + "\t\t" + users[i].name + "\t" + users[i].surname);
		}

		if (isempty)
			puttext("\n\n\t\tList is empty!");

		puttext("\n");
		if (page > 0)
			putbutton("previous", "Previous page");
		if (page < ((int)users.size() - 1) / 5)
			putbutton("next", "Next page");
		putbutton("ESCAPE", "Back");
		puttext("  Page " + to_string(page + 1) + " of " + to_string(((int)users.size() - 1) / 5 + 1));


		string cmd = waitforuserinput();

		if (cmd == "adduser")
		{
			showedituserpage();
		}
		else if (cmd == "adddriver")
		{
			showeditdriverpage();
		}
		else if (cmd == "search")
		{
			User user = getuser(getelement("username").text);
			if (user.id == 0)
				error = "  Username does not exist!";
			else
				showuserinfopage(user.id);
		}
		else if (cmd == "previous")
		{
			page--;
		}
		else if (cmd == "next")
		{
			page++;
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
		else
		{
			int index = stoi(cmd);
			showuserinfopage(users[index].id);
		}
		clearlayout();
	}
	closelayout();
}

void showuserinfopage(int userid)
{
	newlayout("Information of user #" + to_string(userid));

	while (true)
	{
		User user = getuser(userid);

		puttextbox("type", "User type", usertypetostring(user.type), false, true);
		puttextbox("username", "Username", user.username, false, true);
		puttextbox("password", "Password", user.password, true, true);

		puttextbox("name", "Name", user.name, false, true);
		puttextbox("surname", "Surname", user.surname, false, true);
		puttextbox("phonenumber", "Phone number", user.phonenumber, false, true);
		if (user.refererid != 0)
			putbutton("referer", "Referer user #" + to_string(user.refererid));

		puttextbox("cardnumber", "Card number", user.cardnumber, false, true);
		puttextbox("cardpin", "Card PIN", user.cardpin, false, true);

		putbutton("edit", "Edit user");
		putbutton("remove", "Remove user", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);
		putbutton("ESCAPE", "Back");


		string cmd = waitforuserinput();

		if (cmd == "referer")
		{
			showuserinfopage(user.refererid);
		}
		else if (cmd == "edit")
		{
			if (user.type == DriverUser)
				showeditdriverpage(userid);
			else
				showedituserpage(userid);
		}
		else if (cmd == "remove")
		{
			newlayout("Delete confirm");

			puttext("\n\n\t\t\t\tAre you sure you want to delete this user?");
			putbutton("ESCAPE", "No");
			putbutton("yes", "Yes", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);
			cmd = waitforuserinput();
			if (cmd == "yes")
			{
				deleteuser(userid);
				closelayout();
				break;
			}
			else
				closelayout();
		}
		else if (cmd == "ESCAPE")
			break;
		clearlayout();
	}

	closelayout();
}

void showedituserpage(int userid)
{
	User user = getuser(userid);

	newlayout(userid == 0 ? "Add new user" : "Edit user #" + to_string(userid));

	puttextbox("username", "Username", user.username);
	puttextbox("password", "Password", user.password, true);

	puttextbox("name", "Name", user.name);
	puttextbox("surname", "Surname", user.surname);
	puttextbox("phonenumber", "Phone number", user.phonenumber);
	puttextbox("referer", "Referer", getuser(user.refererid).username);

	puttextbox("cardnumber", "Card number", user.cardnumber);
	puttextbox("cardpin", "Card PIN", user.cardpin);

	putbutton("apply", userid == 0 ? "Add user" : "Apply", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
	putbutton("ESCAPE", "Back");
	puttext("\n");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();
		string &errorstext = getelement("errors").text;
		errorstext = "";

		if (cmd == "apply")
		{
			bool error = false;
			validatefield("username");
			validatefield("password");
			validatefield("name");
			validatefield("surname");
			validatefield("phonenumber");
			validatefield("cardnumber");
			validatefield("cardpin");

			int refererid = 0;
			string refererusername = getelement("referer").text;
			if (refererusername != "")
			{
				User referer = getuser(refererusername);
				if (referer.id == 0) {
					error = true;
					errorstext += "\nThe provided referer doesn't exist!";
				}
				else
					refererid = referer.id;
			}

			if (!error)
			{
				User user = {
					userid, RegularUser,
					getelement("username").text,
					getelement("password").text,
					getelement("cardnumber").text,
					getelement("cardpin").text,
					getelement("name").text,
					getelement("surname").text,
					getelement("phonenumber").text,
					refererid
				};
				Error e = (userid == 0 ? registeruser(user) : edituser(user));
				if (e == Success)
				{
					newlayout("Message");
					puttext("\n\n\t\t\t\tOperation has successfuly completed!");
					putbutton("", "OK", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
					waitforuserinput();
					closelayout();
					break;
				}
				else if (e == ExistingUserName)
					errorstext += "\nThis username is taken. Please choose another one.";
				else if (e == InvalidCardNumber)
					errorstext += "\nThe provided card number is invalid!";
				else if (e == InvalidCardPin)
					errorstext += "\nThe provided card PIN is invalid!";
				else
					errorstext += "\nUnknown error!!!";
			}
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
	}

	closelayout();
}

void showeditdriverpage(int userid)
{
	User user = getuser(userid);

	newlayout(userid == 0 ? "Add new driver" : "Edit driver #" + to_string(userid));

	puttextbox("username", "Username", user.username);
	puttextbox("password", "Password", user.password, true);

	puttextbox("name", "Name", user.name);
	puttextbox("surname", "Surname", user.surname);
	puttextbox("phonenumber", "Phone number", user.phonenumber);

	puttextbox("cardnumber", "Card number", user.cardnumber);
	puttextbox("cardpin", "Card PIN", user.cardpin);

	putbutton("apply", userid == 0 ? "Add driver" : "Apply", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
	putbutton("ESCAPE", "Back");
	puttext("\n");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();
		string &errorstext = getelement("errors").text;
		errorstext = "";

		if (cmd == "apply")
		{
			bool error = false;
			validatefield("username");
			validatefield("password");
			validatefield("name");
			validatefield("surname");
			validatefield("phonenumber");
			validatefield("cardnumber");
			validatefield("cardpin");

			if (!error)
			{
				User user = {
					userid, DriverUser,
					getelement("username").text,
					getelement("password").text,
					getelement("cardnumber").text,
					getelement("cardpin").text,
					getelement("name").text,
					getelement("surname").text,
					getelement("phonenumber").text,
					0
				};
				Error e = (userid == 0 ? registeruser(user) : edituser(user));
				if (e == Success)
				{
					newlayout("Message");
					puttext("\n\n\t\t\t\tOperation has successfuly completed!");
					putbutton("", "OK", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
					waitforuserinput();
					closelayout();
					break;
				}
				else if (e == ExistingUserName)
					errorstext += "\nThis username is taken. Please choose another one.";
				else if (e == InvalidCardNumber)
					errorstext += "\nThe provided card number is invalid!";
				else if (e == InvalidCardPin)
					errorstext += "\nThe provided card PIN is invalid!";
				else
					errorstext += "\nUnknown error!!!";
			}
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
	}

	closelayout();
}

void showmanagetripspage()
{
	int page = 0;

	newlayout("Manage trips");

	while (true)
	{
		vector<Trip> trips = gettrips();

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
			showedittrippage(0);
		}
		else if (cmd == "previous")
		{
			page--;
		}
		else if (cmd == "next")
		{
			page++;
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
		else
		{
			int index = stoi(cmd);
			showtripinfopage(trips[index].id);
		}
		clearlayout();
	}
	closelayout();
}

void showtripinfopage(int tripid)
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
		puttextbox("canceled", "Is canceled", trip.canceled ? "Yes" : "No" , false, true);
		putbutton("driver", "Driver user #" + to_string(trip.driverid)); puttext("");

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

		if (cmd == "driver")
		{
			showuserinfopage(trip.driverid);
		}
		else if (cmd == "edit")
		{
			showedittrippage(tripid);
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
			showticketinfopage(tickets[index].id);
		}

		clearlayout();
	}

	closelayout();
}

void showedittrippage(int tripid)
{
	Trip trip = gettrip(tripid);

	newlayout(tripid == 0 ? "Define new trip" : "Edit trip #" + to_string(tripid));

	puttextbox("origin", "Origin", to_string(trip.origincity));
	puttextbox("destination", "Destination", to_string(trip.destinationcity));
	puttextbox("time", "Departure time (y/m/d h:m)", tripid == 0 ? "" : tmtostring(trip.departuretime));
	puttextbox("vehicle", "Vehicle (Bus/Train/Airplane)", vehicletostring(trip.vehicle));
	puttextbox("driver", "Driver username", getuser(trip.driverid).username);

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
				int driverid = getuser(getelement("driver").text).id;
				if (driverid == 0)
					getelement("errors").text = "\n\nDriver does no exist!";
				else
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

void showticketinfopage(int ticketid)
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
	puttextbox("", "Is canceled", ticket.canceled ? "Yes" : "No", false, true);
	putbutton("holder", "Holder user #" + to_string(ticket.holderid)); puttext("");
	putbutton("ESCAPE", "Back");

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "holder")
			showuserinfopage(ticket.holderid);
		else if (cmd == "ESCAPE")
			break;
	}
	closelayout();
}

void showmanagediscountcodespage()
{
	newlayout("Manage discount codes");
	int page = 0;

	while (true)
	{
		vector<DiscountCode> codes = getdiscountcodes();

		putbutton("add", "Add discount code");
		puttext("\n\nDiscount codes:");

		bool isempty = true;
		for (int i = page * 5; i < codes.size() && i < (page + 1) * 5; i++)
		{
			isempty = false;
			puttextbox("", codes[i].code, to_string(codes[i].discountprecentage) + "%", false, true);
			putbutton(to_string(i), "Remove");
		}
		if (isempty)
			puttext("\n\n\t\tList is empty!");

		puttext("\n");
		if (page > 0)
			putbutton("previous", "Previous page");
		if (page < ((int)codes.size() - 1) / 5)
			putbutton("next", "Next page");
		putbutton("ESCAPE", "Back");
		puttext("  Page " + to_string(page + 1) + " of " + to_string(((int)codes.size() - 1) / 5 + 1));

		string cmd = waitforuserinput();

		if (cmd == "add")
		{
			newlayout("Define new discount code");

			puttextbox("code", "Code", "");
			puttextbox("discount", "Discount precentage (%)", "");
			putbutton("add", "Add");
			putbutton("ESCAPE", "Back");
			puttext("errors", "", BackColorWhite | ForeColorRed);

			while (true)
			{
				try
				{
					string cmd = waitforuserinput();
					if (cmd == "add")
					{
						if (!definediscountcode(getelement("code").text, stoi(getelement("discount").text)))
							getelement("errors").text = "\n\nDiscount code already exists!";
						else
							break;
					}
					else if (cmd == "ESCAPE")
						break;
				}
				catch (exception)
				{
					getelement("errors").text = "\n\nInvalid input!";
				}
			}
			closelayout();
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
			deletediscountcode(codes[index].code);
		}
		clearlayout();
	}

	closelayout();
}
