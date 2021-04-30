#include "vehicleinformation.h"

#include <fstream>

using namespace std;

VehicleInformation info;

void savevehicleinformation()
{
	
	ofstream file;
	file.open("vehicleinformation.txt",ios::trunc);

	file << info.buscostcoefficient <<endl;
	file << info.traincostcoefficient <<endl;
	file << info.airplanecostcoefficient <<endl;
	file << info.busspeed <<endl;
	file << info.trainspeed <<endl;
	file << info.airplanespeed <<endl;
	file << info.buscapacity <<endl;
	file << info.traincapacity <<endl;
	file << info.airplanecapacity <<endl;
	file << info.buswaitinglistcapacity <<endl;
	file << info.trainwaitinglistcapacity <<endl;
	file << info.airplanewaitinglistcapacity <<endl;

	file.close();
}

void loadvehicleinformation()
{
	ifstream file;
	file.open("vehicleinformation.txt");

	if (file >> info.buscostcoefficient)
	{
		file >> info.traincostcoefficient ;
		file >> info.airplanecostcoefficient ;
		file >> info.busspeed ;
		file >> info.trainspeed ;
		file >> info.airplanespeed ;
		file >> info.buscapacity ;
		file >> info.traincapacity ;
		file >> info.airplanecapacity ;
		file >> info.buswaitinglistcapacity ;
		file >> info.trainwaitinglistcapacity ;
		file >> info.airplanewaitinglistcapacity ;
		file.close();
	}
	else
	{
		info = VehicleInformation();
		file.close();
		savevehicleinformation();
	}
}

VehicleInformation & getvehicleinformation()
{
	return info;
}

void editvehicleinformation(VehicleInformation vehicleinformation)
{
	info = vehicleinformation;
	savevehicleinformation();
}
