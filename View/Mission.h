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
	void restart();
	bool chooseMission(string mission);
	void missionUpdate(Daniable* victim, string attacker);
	string manageMissionInitialSynch();
	string manageMissionChange();
	Daniable* manageAttack(pair <int,int> tile);
	bool isGameOver();
	bool hasGameOverMessageBeenSent();

private:
	bool gameOverMessageSent;
	FlagMission flagMission;
	DeathmatchMission deathmatch;

};

#endif