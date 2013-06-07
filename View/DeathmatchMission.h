#pragma once
#ifndef __DEATHMATCHMISION_H__
#define __DEATHMATCHMISION_H__

#include "Personaje.h"


class DeathmatchMision {

public:
	DeathmatchMision();
	~DeathmatchMision();
	void initialize();
	bool isTheChosenMision();
	void updateMissionStatus(Daniable* victim, Daniable* attacker);
	bool missionEnded();
	void choose();

private:
	bool chosen;

};

#endif