#include "waitinglist.h"
#include "trip.h"
#include "ticketing.h"
#include "vehicleinformation.h"

#include <fstream>

vector<WaitingList> waintinglists;

void savewaitinglists()
{
	ofstream file;
	file.open("waitinglist.txt", ios::trunc);
	for (int i = 0; i < waintinglists.size(); i++)
	{
		WaitingList w = waintinglists[i];
		file << w.tripid << '\t';
		file << w.users.size();
		for (int j = 0; j < w.users.size(); j++)
			file << '\t' << w.users[j];
		file << endl;
	}
	file.close();
}

void loadwaitinglists()
{
	ifstream file;
	file.open("waitinglist.txt");
	WaitingList w;
	while (file >> w.tripid)
	{
		int n;
		file >> n;
		for (int i = 0; i < n; i++)
		{
			int userid;
			file >> userid;
			w.users.push_back(userid);
		}
		waintinglists.push_back(w);
	}
	file.close();
}

void updatewaitinglists()
{
	for (int i = 0; i < waintinglists.size(); i++)
	{
		if (waintinglists[i].users.size() == 0)
			continue;

		vector<int> emptyseats = getemptyseats(waintinglists[i].tripid);

		if (emptyseats.size() > 0)
		{
			Ticket ticket{
				0,
				waintinglists[i].tripid,
				waintinglists[i].users[0],
				emptyseats[0],
			};
			if (bookticket(ticket) == Success)
			{
				waintinglists[i].users.erase(waintinglists[i].users.begin());
				savewaitinglists();
			}
		}
	}
}

bool addusertowaitinglist(int userid, int tripid)
{
	int capacity = 0;
	switch (gettrip(tripid).vehicle)
	{
	case Bus:
		capacity = getvehicleinformation().buscapacity;
		break;
	case Train:
		capacity = getvehicleinformation().traincapacity;
		break;
	case Airplane:
		capacity = getvehicleinformation().airplanecapacity;
		break;
	}

	for (int i = 0; i < waintinglists.size(); i++)
	{
		if (waintinglists[i].tripid == tripid)
		{
			if (capacity <= waintinglists[i].users.size())
				return false;

			waintinglists[i].users.push_back(userid);
			savewaitinglists();
			return true;
		}
	}

	// the trip is not in the list.
	WaitingList newlist;
	newlist.tripid = tripid;
	newlist.users.push_back(userid);
	waintinglists.push_back(newlist);
	savewaitinglists();
	return true;
}

bool removeuserfromwaitinglist(int userid, int tripid)
{
	for (int i = 0; i < waintinglists.size(); i++)
	{
		if (waintinglists[i].tripid == tripid)
		{
			for (int j = 0; j < waintinglists[i].users.size(); j++)
			{
				if (waintinglists[i].users[j] == userid)
				{
					waintinglists[i].users.erase(waintinglists[i].users.begin() + j);
					savewaitinglists();
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

bool isuserinwaitinglist(int userid, int tripid)
{
	for (int i = 0; i < waintinglists.size(); i++)
	{
		if (waintinglists[i].tripid == tripid)
		{
			for (int j = 0; j < waintinglists[i].users.size(); j++)
			{
				if (waintinglists[i].users[j] == userid)
					return true;
			}
			return false;
		}
	}
	return false;
}

vector<int> getusersinwaitinglist(int tripid)
{
	for (int i = 0; i < waintinglists.size(); i++)
	{
		if (waintinglists[i].tripid == tripid)
		{
			return waintinglists[i].users;
		}
	}
	return vector<int>();
}

vector<int> gettripsinwaitinglist(int userid)
{
	vector<int> trips;
	for (int i = 0; i < waintinglists.size(); i++)
	{
		for (int j = 0; j < waintinglists[i].users.size(); j++)
		{
			if (waintinglists[i].users[j] == userid)
			{
				trips.push_back(waintinglists[i].tripid);
			}
		}
	}
	return trips;
}
