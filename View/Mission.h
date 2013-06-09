#ifndef __MISSION_H__
#define __MISSION_H__

#include "FlagMission.h"
#include "Daniable.h"
#include "DeathmatchMission.h"

using namespace std;


class Mission {

public:
	Mission();
	~Mission();
	void initialize();
	bool chooseMission(string mission);
	void missionUpdate(Daniable* victim, Daniable* attacker);
	string manageMissionInitialSynch();
	string manageMissionChange();
	Daniable* manageAttack(pair <int,int> tile);
	bool isGameOver();

private:
	bool gameOverMessageSent;
	FlagMission flagMission;
	DeathmatchMission deathmatch;

};

#endif