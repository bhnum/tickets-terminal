#include "tariffs.h"
#include "vehicleinformation.h"

#include <fstream>
#include <cmath>

vector<UserDiscount> userdiscounts;
vector<DiscountCode> discountcodes;
vector<CityInfo> cities;

void savetarrifs()
{
	ofstream ufile;
	ufile.open("userdiscounts.txt", ios::trunc);
	for (int i = 0; i < userdiscounts.size(); i++)
	{
		ufile << userdiscounts[i].userid << '\t';
		int n = userdiscounts[i].discounts.size();
		ufile << n;
		for (int j = 0; j < n; j++)
		{
			ufile << '\t' << userdiscounts[i].discounts[j];
		}
		ufile << endl;
	}
	ufile.close();

	ofstream dfile;
	dfile.open("discountcodes.txt", ios::trunc);
	for (int i = 0; i < discountcodes.size(); i++)
	{
		dfile << discountcodes[i].code << '\t';
		dfile << discountcodes[i].discountprecentage << endl;
	}
	dfile.close();
}

void loadtariffs()
{
	ifstream ufile;
	ufile.open("userdiscounts.txt");
	UserDiscount u;
	while (ufile >> u.userid)
	{
		int n;
		ufile >> n;
		for (int i = 0; i < n; i++)
		{
			int p;
			ufile >> p;
			u.discounts.push_back(p);
		}
		userdiscounts.push_back(u);
	}
	ufile.close();

	ifstream dfile;
	dfile.open("discountcodes.txt");
	DiscountCode d;
	while (getline(dfile, d.code, '\t'))
	{
		dfile >> d.discountprecentage;
		discountcodes.push_back(d);

		getline(dfile, d.code); // remove new line character.
	}
	dfile.close();

	ifstream cfile;
	cfile.open("cities.txt");
	CityInfo c;
	while (cfile >> c.number)
	{
		cfile >> c.x >> c.y;
		cities.push_back(c);
	}
	cfile.close();
}

double gettripdistance(int origincity, int destinationcity)
{
	return sqrt(pow(cities[origincity].x - cities[destinationcity].x, 2)
		      + pow(cities[origincity].y - cities[destinationcity].y, 2));
}

double gettriptime(int origincity, int destinationcity, Vehicle vehicle)
{
	switch (vehicle)
	{
	case Bus:
		return gettripdistance(origincity, destinationcity) / getvehicleinformation().busspeed;
	case Train:
		return gettripdistance(origincity, destinationcity) / getvehicleinformation().trainspeed;
	case Airplane:
		return gettripdistance(origincity, destinationcity) / getvehicleinformation().airplanespeed;
	default:
		return 0;
	}
}

int getcostofticket(int tripid, int userid, string discountcode)
{
	Trip trip = gettrip(tripid);
	double discount = userid == 0 ? 0 : 0.1;
	if (discountcode != "")
	{
		discount = getdiscountofcode(discountcode) / 100.0;
	}
	else
	{
		UserDiscount d = getuserdiscountsof(userid);
		if (d.discounts.size() > 0)
			discount = d.discounts[0] / 100.0;
	}

	int coefficient = 0;
	switch (trip.vehicle)
	{
	case Bus:
		coefficient = getvehicleinformation().buscostcoefficient;
		break;
	case Train:
		coefficient = getvehicleinformation().traincostcoefficient;
		break;
	case Airplane:
		coefficient = getvehicleinformation().airplanecostcoefficient;
		break;
	}

	double cost = gettripdistance(trip.origincity, trip.destinationcity) * coefficient * (1 - discount);
	return round(cost / 100) * 100;
}

void adduserdiscount(int userid, int discountprecentage)
{
	for (int i = 0; i < userdiscounts.size(); i++)
	{
		if (userdiscounts[i].userid == userid)
		{
			userdiscounts[i].discounts.push_back(discountprecentage);
			savetarrifs();
			return;
		}
	}
	userdiscounts.push_back({ userid, vector<int>({ discountprecentage }) });
	savetarrifs();
}

void consumeuserdiscount(int userid)
{
	for (int i = 0; i < userdiscounts.size(); i++)
	{
		if (userdiscounts[i].userid == userid)
		{
			if (userdiscounts[i].discounts.size() > 0)
			{
				userdiscounts[i].discounts.erase(userdiscounts[i].discounts.begin());
				savetarrifs();
			}
			return;
		}
	}
}

bool definediscountcode(string code, int discountprecentage)
{
	if (getdiscountofcode(code) != -1)
		return false;
	discountcodes.push_back({ code, discountprecentage });
	savetarrifs();
	return true;
}

bool editdiscountcode(string code, int discountprecentage)
{
	for (int i = 0; i < discountcodes.size(); i++)
	{
		if (discountcodes[i].code == code)
		{
			discountcodes[i].discountprecentage = discountprecentage;
			return true;
		}
	}
	return false;
}

bool deletediscountcode(string code)
{
	for (int i = 0; i < discountcodes.size(); i++)
	{
		if (discountcodes[i].code == code)
		{
			discountcodes.erase(discountcodes.begin() + i);
			return true;
		}
	}
	return false;
}

int getdiscountofcode(string code)
{
	for (int i = 0; i < discountcodes.size(); i++)
	{
		if (discountcodes[i].code == code)
			return discountcodes[i].discountprecentage;
	}
	return -1;
}

UserDiscount getuserdiscountsof(int userid)
{
	for (int i = 0; i < userdiscounts.size(); i++)
	{
		if (userdiscounts[i].userid == userid)
			return userdiscounts[i];
	}
	return { 0 };
}

vector<UserDiscount>& getuserdiscounts()
{
	return userdiscounts;
}

vector<DiscountCode>& getdiscountcodes()
{
	return discountcodes;
}
