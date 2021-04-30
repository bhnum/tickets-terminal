#include "users.h"
#include "accounts.h"
#include "tariffs.h"

#include <fstream>

vector<User> users;

static int generateid()
{
	// ids start from 1
	int max = 0;
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].id > max)
			max = users[i].id;
	}
	return max + 1;
}

void saveusers()
{
	ofstream file;
	file.open("users.txt", ios::trunc);
	for (int i = 0; i < users.size(); i++)
	{
		User &u = users[i];
		file << u.id << ",";
		file << (int)u.type << ",";
		file << u.username << ",";
		file << u.password << ",";
		file << u.cardnumber << ",";
		file << u.cardpin << ",";
		file << u.name << ",";
		file << u.surname << ",";
		file << u.phonenumber << ",";
		file << u.refererid << endl;
	}
	file.close();
}

void loadusers()
{
	ifstream file;
	file.open("users.txt");
	User u;
	while (file >> u.id)
	{
		file.ignore();
		file >> (int &)u.type;
		file.ignore();
		getline(file, u.username, ',');
		getline(file, u.password, ',');
		getline(file, u.cardnumber, ',');
		getline(file, u.cardpin, ',');
		getline(file, u.name, ',');
		getline(file, u.surname, ',');
		getline(file, u.phonenumber, ',');
		file >> u.refererid;
		users.push_back(u);
	}
	file.close();

	if (users.size() == 0)
	{
		users.push_back({ 1, AdminUser, "admin", "admin" });
		saveusers();
	}
}

Error registeruser(User &user)
{
	// We assume that the provided fields are not empty.

	// Guset users have empty usernames.
	if (user.type != GuestUser && getuser(user.username).id != 0)
		return ExistingUserName;

	Error e = validatepin(user.cardnumber,user.cardpin);
	if (e != Success)
		return e;

	user.id = generateid();
	users.push_back(user);
	saveusers();

	if (user.refererid != 0)
	{
		adduserdiscount(user.id, 50);
		adduserdiscount(user.id, 50);
		adduserdiscount(user.refererid, 100);
	}

	return Success;
}

Error edituser(User user)
{
	// We assume that the provided fields are not empty.

	int duplicateid = getuser(user.username).id;
	if (duplicateid != 0 && duplicateid != user.id)
		return ExistingUserName;

	if (user.type != AdminUser)
	{
		Error e = validatepin(user.cardnumber, user.cardpin);
		if (e != Success)
			return e;
	}

	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].id == user.id)
		{
			users[i] = user;
			saveusers();
			return Success;
		}
	}
	return NonExistentRecord;
}

Error deleteuser(string username)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == username)
		{
			users.erase(users.begin() + i);
			saveusers();
			return Success;
		}
	}
	return NonExistentRecord;
}

Error deleteuser(int id)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].id == id)
		{
			users.erase(users.begin() + i);
			saveusers();
			return Success;
		}
	}
	return NonExistentRecord;
}

User getuser(string username)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == username)
			return users[i];
	}
	return { 0 };
}

User getuser(int id)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].id == id)
			return users[i];
	}
	return { 0 };
}

vector<User>& getusers()
{
	return users;
}

vector<User> getusers(UserType type)
{
	vector<User> oftype;
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].type == type)
			oftype.push_back(users[i]);
	}
	return oftype;
}

User getadmin()
{
	return getusers(AdminUser)[0];
}
