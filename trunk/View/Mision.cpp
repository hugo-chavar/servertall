#include "Mision.h"
#include "GameView.h"

Mision::Mision() { }

Mision::~Mision() { }

void Mision::initialize() { // FLAGMISION DEFAULT EN CASO DE NO PASAR LA MISION ?
	if (flagMision.isTheChosenMision())
		flagMision.initialize();
//	else
//		if (deathmatchMision.isTheChosenMision())
//			deathmatchMision.initialize();
}

void Mision::chooseMision(string mision) {
	if (mision.compare("flagMision") == 0)
		flagMision.choose();
	//else
	//	if (mision.compare("deathmatchMision") == 0)
	//		deathmatchMision.choose();
}

bool Mision::isGameOver() {
	if (flagMision.isTheChosenMision()) {
		if (flagMision.allFlagsDestroyed()) {
			return true;
		}
	}
	//else
	//	if (deathmatchMision.isTheChosenMision()) {
	//		if (...)
	//			return true;
	//	}
	return false;
}

string Mision::manageMisionInitialSynch() {
	// IMPLEMENTAR
	return "";
}