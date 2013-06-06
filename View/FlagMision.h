#pragma once
#ifndef __FLAGMISION_H__
#define __FLAGMISION_H__

#include <vector>
#include "Flag.h"


class FlagMision {

public:
	FlagMision();
	~FlagMision();
	void initialize();
	bool isTheChosenMision();
	void choose();
	void hurtFlag(Flag* flag, float damage, string userID);
	bool allFlagsDestroyed();

private:
	int findFlag(Flag* flag);
	vector <Flag*> flags;
	bool chosen;

};

#endif