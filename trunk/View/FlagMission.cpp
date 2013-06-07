#include "FlagMission.h"
#include "GameView.h"
#include "Daniable.h" // VER


FlagMission::FlagMission() {
	chosen = true; // HARCODEADO
}

FlagMission::~FlagMission() {
	for (unsigned int i = 0; i < flags.size(); i++) {
		delete flags[i];
	}
	flags.clear();
}

void FlagMission::initialize() {
	int numberOfFlags = 1; // HARCODEADO
	for (int i=0; i<numberOfFlags; i++) {
		flags.push_back(new Flag());
	}
}

bool FlagMission::isTheChosenMission() {
	return chosen;
}

void FlagMission::choose() {
	chosen = true;
}

int FlagMission::findFlag(Flag* flag) {
	for (unsigned int i=0; i<flags.size(); i++) {
		if (flags[i] == flag)
			return i;
	}
	return -1;
}

bool FlagMission::isAFlag(Daniable* entity) {
	for (unsigned int i=0; i<flags.size(); i++) {
		if (flags[i] == entity) {
			return true;
		}					
	}
	return false;
}

void FlagMission::updateMissionStatus(Daniable* victim, Daniable* attacker) {
	if (isAFlag(victim)) {
		Flag* flag = static_cast<Flag*>(victim);
		if (!flag->isAlive()) {
			int flagNumber = findFlag(flag);
			flags[flagNumber]->destroy();
			delete flags[flagNumber];
			flags.erase(flags.begin()+flagNumber);
			int score = GameView::instance().GameView::instance().findPlayerByPersonaje(attacker)->missionScore();
			GameView::instance().findPlayerByPersonaje(attacker)->missionScore(score+1);
		}
	}
}

bool FlagMission::allFlagsDestroyed() {
	return flags.empty();
}

string FlagMission::initToString() {
	string argument = "flagMission;"+flags[0]->getName()+";";
	for(unsigned int i=0; i<this->flags.size(); i++)
	{
		argument += stringUtilities::pairIntToString(flags[i]->position())+";";
	}
	if(argument.size()>0)
		argument.erase(argument.size()-1);
	return argument;
}

vector <Flag*>* FlagMission::getFlags() {
	return &flags;
}

Daniable* FlagMission::manageFlagAttack(pair <int,int> tile) {
	vector <Flag*>::iterator it;
	for (it=flags.begin(); it!=flags.end(); it++)
		if ((*it)->position() == tile)
			return *it;
	return NULL;
}