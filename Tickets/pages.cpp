#include "pages.h"

vector<int> getemptyseats(int tripid)
{
	vector<Ticket> tickets = gettickets(tripid);
	int seats;
	switch (gettrip(tripid).vehicle)
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

	vector<int> emptyseats;
	for (int j = 0; j < seats; j++)
		emptyseats.push_back(j + 1);
	for (int j = 0; j < tickets.size(); j++)
	{
		for (int k = 0; k < emptyseats.size(); k++)
		{
			if (tickets[j].seatnumber == emptyseats[k])
				emptyseats.erase(emptyseats.begin() + k);
		}
	}
	return emptyseats;
}

void showmainpage()
{
	newlayout("tickets.com - Online Ticketing Service!");

	putbutton("bookticket", "Book a ticket"); puttext("");
	putbutton("trackticket", "Track a ticket"); puttext("");

	putbutton("login", "Log in", BackColorBlue | ForeColorWhite, BackColorLightBlue | ForeColorWhite);
	putbutton("register", "Register", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
	putbutton("ESCAPE", "Exit", BackColorRed | ForeColorWhite, BackColorLightRed | ForeColorWhite);

	while (true)
	{
		string cmd = waitforuserinput();

		if (cmd == "bookticket")
		{
			showselecttrippage(0);
		}
		else if (cmd == "trackticket")
		{
			showtrackticketpage(0);
		}
		else if (cmd == "register")
		{
			showregisterpage();
		}
		else if (cmd == "login")
		{
			showloginpage();
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
	}

	closelayout();
}

void showloginpage()
{
	newlayout("Log in");

	puttextbox("username", "Username", "");
	puttextbox("password", "Password", "", true);

	putbutton("login", "Log in", BackColorBlue | ForeColorWhite, BackColorLightBlue | ForeColorWhite);
	putbutton("register", "Register", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
	putbutton("ESCAPE", "Back");
	puttext("\n");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();
		string &errorstext = getelement("errors").text;
		errorstext = "";

		if (cmd == "login")
		{
			bool error = false;
			validatefield("username");
			validatefield("password");

			if (!error)
			{
				User user = getuser(getelement("username").text);
				if (user.id == 0)
					errorstext += "\nUsername doesn't exist!";
				else if (user.password != getelement("password").text)
					errorstext += "\nPassword is incorrect!";
				else
				{
					if (user.type == AdminUser)
						showadminpage(user.id);
					else if (user.type == DriverUser)
						showdriverpage(user.id);
					else if (user.type == RegularUser)
						showuserpage(user.id);
					break;
				}
			}
		}
		else if (cmd == "register")
		{
			showregisterpage();
			break;
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
	}
	closelayout();
}

void showregisterpage()
{
	newlayout("Register");

	puttextbox("username", "Username", "");
	puttextbox("password", "Password", "", true);

	puttextbox("name", "Name", "");
	puttextbox("surname", "Surname", "");
	puttextbox("phonenumber", "Phone number", "");
	puttextbox("referer", "Referer", "");

	puttextbox("cardnumber", "Card number", "");
	puttextbox("cardpin", "Card PIN", "");

	putbutton("register", "Register", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);
	putbutton("ESCAPE", "Back");
	puttext("\n");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();
		string &errorstext = getelement("errors").text;
		errorstext = "";

		if (cmd == "register")
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
					0, RegularUser,
					getelement("username").text,
					getelement("password").text,
					getelement("cardnumber").text,
					getelement("cardpin").text,
					getelement("name").text,
					getelement("surname").text,
					getelement("phonenumber").text,
					refererid
				};
				Error e = registeruser(user);
				if (e == Success)
				{
					showuserpage(user.id);
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

void showeditinfopage(int userid)
{
	User user = getuser(userid);

	newlayout("Edit your information");

	puttextbox("username", "Username", user.username);
	puttextbox("password", "Password", user.password, true);

	puttextbox("name", "Name", user.name);
	puttextbox("surname", "Surname", user.surname);
	puttextbox("phonenumber", "Phone number", user.phonenumber);

	puttextbox("cardnumber", "Card number", user.cardnumber);
	puttextbox("cardpin", "Card PIN", user.cardpin);

	putbutton("edit", "Edit", BackColorBlue | ForeColorWhite, BackColorLightBlue | ForeColorWhite);
	putbutton("ESCAPE", "Back");

	puttext("\n");
	puttext("errors", "", BackColorWhite | ForeColorRed);

	while (true)
	{
		string cmd = waitforuserinput();

		string &errorstext = getelement("errors").text;
		if (cmd == "edit")
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
				User edited = {
					userid, user.type,
					getelement("username").text,
					getelement("password").text,
					getelement("cardnumber").text,
					getelement("cardpin").text,
					getelement("name").text,
					getelement("surname").text,
					getelement("phonenumber").text,
					user.refererid
				};
				Error e = edituser(edited);
				if (e == Success) {
					newlayout("Message");
					puttext("\n\n\t\t\t\tYour information has been successfuly changed!");
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
			break;
	}

	closelayout();
}
