#pragma once
#ifndef __FLAGMISION_H__
#define __FLAGMISION_H__

#include "Flag.h"
#include "Personaje.h"


class FlagMision {

public:
	FlagMision();
	~FlagMision();
	void initialize(int stageWidth, int stageHeight);
	bool isTheChosenMision();
	void hurtFlag(Flag* flag, float damage, string userID);
	bool allFlagsDestroyed();

private:
	int findFlag(Flag* flag);
	vector <Flag*> flags;
	bool chosen;

};

#endif