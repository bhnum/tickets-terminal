#pragma once

#include <string>
#include <vector>

#include "commons.h"

using namespace std;

enum UserType {
	GuestUser,
	RegularUser,
	DriverUser,
	AdminUser,
};

struct User {
	int id; // Starts from 1
	UserType type;

	string username;
	string password;
	string cardnumber;
	string cardpin;

	string name;
	string surname;
	string phonenumber;
	int refererid;
};

// Load users list from users.txt.
void loadusers();

// For guest users pass cardnumber, cardpin, name, surname, phonenumber.
// For regular users pass username, password, cardnumber, cardpin, name, surname, phonenumber, and refererid.
// For driver users pass username, password, cardnumber, cardpin, name, surname, phonenumber.
// For admin users pass username, password, cardnumber.
// May return Success, EmptyField, ExistingUserName, NonExistentReferer, InvalidCardNumber or InvalidCardPin Errors.
Error registeruser(User &user);

// May return Success, EmptyField, ExistingUserName, NonExistentRecord, InvalidCardNumber or InvalidCardPin Errors.
// The id field should not be changed.
Error edituser(User user);

Error deleteuser(string username);

Error deleteuser(int id);

// If not found returns { 0 }
User getuser(string username);

// If not found returns { 0 }
User getuser(int id);

vector<User> &getusers();

vector<User> getusers(UserType type);

User getadmin();

static inline string usertypetostring(UserType type)
{
	switch (type)
	{
	case GuestUser:
		return "Guest";
	case RegularUser:
		return "User";
	case DriverUser:
		return "Driver";
	case AdminUser:
		return "Administrator";
	}
}
