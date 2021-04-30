#pragma once

#include <string>
#include <vector>

using namespace std;

struct WaitingList {
	int tripid;
	vector<int> users;
};

// Load waiting list from waitinglist.txt in the following format:
//		tripid		number of waiting users		userid1		userid2		...
void loadwaitinglists();

void updatewaitinglists();

bool addusertowaitinglist(int userid, int tripid);

bool removeuserfromwaitinglist(int userid, int tripid);

bool isuserinwaitinglist(int userid, int tripid);

vector<int> getusersinwaitinglist(int tripid);

vector<int> gettripsinwaitinglist(int userid);
