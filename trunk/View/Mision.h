#ifndef __MISION_H__
#define __MISION_H__

#include "FlagMision.h"

using namespace std;


class Mision {

public:
	Mision();
	~Mision();
	void initialize();
	void chooseMision(string mision);
	string manageMisionInitialSynch();
	bool isGameOver();

private:
	FlagMision flagMision;
	//Deathmatch deathmatch;

};

#endif