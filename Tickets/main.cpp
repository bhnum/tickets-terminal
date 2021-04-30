#include <iostream>
#include <iomanip>

#include "layout.h"

using namespace std;

int main()
{
	//std::ios_base::sync_with_stdio(false);
	//std::setvbuf(stdout, nullptr, _IOFBF, BUFSIZ);

	newlayout("System log in");
	puttextbox("username", "Username", "");
	puttextbox("password", "Password", "", true);
	putbutton("login", "Log in");
	putbutton("loginguest", "Log in as guest");
	putbutton("register", "Register", BackColorGreen | ForeColorWhite, BackColorLightGreen | ForeColorWhite);

	while (true)
	{
		string cmd = waitforuserinput();
		string username = getcomponent("username").text;
		string password = getcomponent("password").text;

		if (cmd == "login")
		{
			newlayout("User panel");
			puttext("You have loged in as " + username + ".");
			puttext("Press ESC to log out.");
			waitforuserinput();
			closelayout();
		}
		else if (cmd == "register")
		{
			newlayout("Register", BackColorRed | ForeColorWhite);
			puttext("Not implemented!!!");
			puttext("Press ESC to go back.");
			waitforuserinput();
			closelayout();
		}
		else if (cmd == "loginguest")
		{
			newlayout("Guest panel");
			puttext("You have logged in as a guest.");
			puttext("Press ESC to log out.");
			waitforuserinput();
			closelayout();
		}
		else if (cmd == "ESCAPE")
		{
			break;
		}
	}
	closelayout();

	return 0;
}