#pragma once

#include <string>
#include <vector>

using namespace std;

// Load waiting list from waitinglist.txt in the following format:
//		tripid		username1	username2	username3	...
void loadwaitinglist();

void addusertowaitinglist(string username, int tripid);

void removeuserfromwaitinglist(string username, int tripid);

vector<string> getusersinwaitinglist(int tripid);

vector<int> gettripsinwaitinglist(string username);
