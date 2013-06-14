#pragma once
#ifndef __FLAGMISSION_H__
#define __FLAGMISSION_H__

#include <vector>
#include "Flag.h"
#include "StringQueue.h"

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
	string changeToString();
	void addChange(string change);
	void updateMissionStatus(Daniable* victim, string attacker);
	vector <Flag*>* getFlags();
	bool isAFlag(Daniable* entity);
	Daniable* manageFlagAttack(pair <int,int> tile);

private:
	StringQueue changes;
	int findFlag(Flag* flag);
	vector <Flag*> flags;
	bool chosen;

};

#endif