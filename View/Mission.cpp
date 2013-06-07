#include "Mission.h"
#include "GameView.h"

Mission::Mission() { }

Mission::~Mission() { }

void Mission::initialize() { // FLAGMission DEFAULT EN CASO DE NO PASAR LA Mission ?
	if (flagMission.isTheChosenMission()) {
		flagMission.initialize();
	} else {
		if (deathmatch.isTheChosenMission()) {
			deathmatch.initialize();
		}
	}
}

void Mission::chooseMission(string mission) {
	if (mission.compare("flagMission") == 0) {
		flagMission.choose();
	}else {
		if (mission.compare("deathmatchMission") == 0)
		deathmatch.choose();
	}
}

void Mission::missionUpdate(Daniable* victim, Daniable* attacker) {
	if (flagMission.isTheChosenMission()) {
		flagMission.updateMissionStatus(victim, attacker);
	} else {
		if (deathmatch.isTheChosenMission()) {
			deathmatch.updateMissionStatus(victim, attacker);
		}
	}
}

bool Mission::isGameOver() {
	if (flagMission.isTheChosenMission()) {
		return (flagMission.allFlagsDestroyed());
	}
	if (deathmatch.isTheChosenMission()) {
		return (deathmatch.missionEnded());
	}
	return false;
}

string Mission::manageMissionInitialSynch() {
	if (flagMission.isTheChosenMission())
		return flagMission.initToString();
	return "";
}

Daniable* Mission::manageAttack(pair <int,int> tile) {
	if (flagMission.isTheChosenMission())
		return flagMission.manageFlagAttack(tile);
}