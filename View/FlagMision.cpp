#include "FlagMision.h"
#include "GameView.h"


FlagMision::FlagMision() {
	chosen = false;
}

FlagMision::~FlagMision() {
	for (unsigned int i = 0; i < flags.size(); i++) {
		delete flags[i];
	}
	flags.clear();
}

void FlagMision::initialize() {
	int numberOfFlags = 5; // HARCODEADO
	for (int i=0; i<numberOfFlags; i++) {
		flags.push_back(new Flag());
	}
}

bool FlagMision::isTheChosenMision() {
	return chosen;
}

void FlagMision::choose() {
	chosen = true;
}

int FlagMision::findFlag(Flag* flag) {
	for (unsigned int i=0; i<flags.size(); i++) {
		if (flags[i] == flag)
			return i;
	}
	return -1;
}

void FlagMision::hurtFlag(Flag* flag, float damage, string userID) {
	flag->hurt(damage);
	if (flag->life() == 0) {
		int flagNumber = findFlag(flag);
		delete flags[flagNumber];
		flags.erase(flags.begin()+flagNumber);
		int score = GameView::instance().findPlayer(userID)->misionScore();
		GameView::instance().findPlayer(userID)->misionScore(score+1);
	}
}

bool FlagMision::allFlagsDestroyed() {
	return flags.empty();
}