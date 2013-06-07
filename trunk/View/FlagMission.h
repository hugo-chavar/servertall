#pragma once
#ifndef __FLAGMISSION_H__
#define __FLAGMISSION_H__

#include <vector>
#include "Flag.h"

using namespace std;


class FlagMission {

public:
	FlagMission();
	~FlagMission();
	void initialize();
	bool isTheChosenMission();
	void choose();
	bool allFlagsDestroyed();
	string initToString();
	void updateMissionStatus(Daniable* victim, Daniable* attacker);
	vector <Flag*>* getFlags();
	bool isAFlag(Daniable* entity);
	Daniable* manageFlagAttack(pair <int,int> tile);

private:
	int findFlag(Flag* flag);
	vector <Flag*> flags;
	bool chosen;

};

#endif