#pragma once

struct VehicleInformation {
	int buscostcoefficient = 40;
	int traincostcoefficient = 100;
	int airplanecostcoefficient = 400;

	int busspeed = 70;
	int trainspeed = 100;
	int airplanespeed = 800;

	int buscapacity = 30;
	int traincapacity = 600;
	int airplanecapacity = 200;

	int buswaitinglistcapacity = 10;
	int trainwaitinglistcapacity = 50;
	int airplanewaitinglistcapacity = 20;
};

void loadvehicleinformation();

VehicleInformation &getvehicleinformation();

void editvehicleinformation(VehicleInformation vehicleinformation);
