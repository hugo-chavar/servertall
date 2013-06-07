#include "Mision.h"
#include "GameView.h"

Mision::Mision() { }

Mision::~Mision() { }

void Mision::initialize() { // FLAGMISION DEFAULT EN CASO DE NO PASAR LA MISION ?
	if (flagMision.isTheChosenMision()) {
		flagMision.initialize();
	} else {
		if (deathmatch.isTheChosenMision()) {
			deathmatch.initialize();
		}
	}
}

void Mision::chooseMision(string mision) {
	if (mision.compare("flagMision") == 0) {
		flagMision.choose();
	}else {
		if (mision.compare("deathmatchMision") == 0)
		deathmatch.choose();
	}
}

void Mision::missionUpdate(Daniable* victim, Daniable* attacker) {
	if (flagMision.isTheChosenMision()) {
		//Algo similar a flag hurt flagMision.
	} else {
		if (deathmatch.isTheChosenMision()) {
			deathmatch.updateMissionStatus(victim, attacker);
		}
	}
}

bool Mision::isGameOver() {
	if (flagMision.isTheChosenMision()) {
		return (flagMision.allFlagsDestroyed());
	}
	if (deathmatch.isTheChosenMision()) {
		return (deathmatch.missionEnded());
	}
	return false;
}

string Mision::manageMisionInitialSynch() {
	// IMPLEMENTAR
	return "";
}