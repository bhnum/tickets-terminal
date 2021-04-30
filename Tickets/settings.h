#pragma once

struct Settings {
	int busspeed = 70;
	int trainspeed = 100;
	int airplanespeed = 800;

	int buscapacity = 30;
	int traincapacity = 500;
	int airplanecapacity = 200;

	int buswaitinglistcapacity = 10;
	int trainwaitinglistcapacity = 50;
	int airplanewaitinglistcapacity = 20;
};

void loadsettings();

Settings &getsettings();

void editSettings(Settings settings);
