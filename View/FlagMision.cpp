#include "FlagMision.h"
#include "GameView.h"

FlagMision::FlagMision() {
	chosen = false;
}

FlagMision::~FlagMision() { }

void FlagMision::initialize(int stageWidth, int stageHeight) {
	int xPosition, yPosition;
	int numberOfFlags = 5; // HARCODEADO
	for (int i=0; i<numberOfFlags; i++) {
		bool validPosition = false;
		while (!validPosition) {
			xPosition = rand() % stageWidth;
			yPosition = rand() % stageHeight;
			KeyPair position;
			position.first = xPosition;
			position.second = yPosition;
			if (!GameView::instance().getWorldView()->getTileAt(position)->hasOtherEntity())
				validPosition = true;
		}
		flags.push_back(new Flag(xPosition, yPosition));
	}
}

bool FlagMision::isTheChosenMision() {
	return chosen;
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
		flags.erase(flags.begin()+findFlag(flag));
		int score = GameView::instance().findPlayer(userID)->misionScore();
		GameView::instance().findPlayer(userID)->misionScore(score+1);
	}
}

bool FlagMision::allFlagsDestroyed() {
	return flags.empty();
}