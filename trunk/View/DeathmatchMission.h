#pragma once
#ifndef __DEATHMATCHMISSION_H__
#define __DEATHMATCHMISSION_H__

#include "Personaje.h"


class DeathmatchMission {

public:
	DeathmatchMission();
	~DeathmatchMission();
	void initialize();
	bool isTheChosenMission();
	void updateMissionStatus(Daniable* victim, string attacker);
	bool missionEnded();
	void choose();
	string getWinner();

private:
	bool chosen;
	string winner;

};

#endif