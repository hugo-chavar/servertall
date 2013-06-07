#ifndef __MISION_H__
#define __MISION_H__

#include "FlagMision.h"
#include "Daniable.h"
#include "DeathmatchMission.h"

using namespace std;


class Mision {

public:
	Mision();
	~Mision();
	void initialize();
	void chooseMision(string mision);
	void missionUpdate(Daniable* victim, Daniable* attacker);
	string manageMisionInitialSynch();
	bool isGameOver();

private:
	FlagMision flagMision;
	DeathmatchMision deathmatch;

};

#endif