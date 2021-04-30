#pragma once

#include <string>
#include <vector>

#include "commons.h"

using namespace std;

enum UserType {
	RegularUser,
	DriverUser,
	AdminUser,
};

struct User {
	UserType type;

	string username;
	string password;
	string cardnumber;
	string cardpin;

	string name;
	string surname;
	string phonenumber;
	string refererusername;
};

// Load users list from users.txt.
void loadusers();

// May return Success, EmptyField, ExistingUserName, InvalidCardNumber or InvalidCardPin Errors.
Error registeruser(User user);

// May return Success, EmptyField, ExistingUserName, InvalidCardNumber or InvalidCardPin Errors.
Error edituser(string oldusername, User user);

Error deleteuser(string username);

User &getuser(string username);

vector<User> &getusers();

vector<User&> getusers(UserType type);
