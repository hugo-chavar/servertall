#include "DeathmatchMission.h"
#include "GameView.h"


#define KILL 3
#define DEATH 2
#define MAX_SCORE 15


DeathmatchMision::DeathmatchMision() {
	chosen = false;
}

DeathmatchMision::~DeathmatchMision() { }

void DeathmatchMision::initialize() {
}

void DeathmatchMision::choose() {
	chosen = true;
}

bool DeathmatchMision::isTheChosenMision() {
	return chosen;
}

void DeathmatchMision::updateMissionStatus(Daniable* victim, Daniable* attacker) {
	Player* playerVictim = NULL;
	Player* playerAttacker = NULL;
	int attackScore = 0;
	int victimScore = 0;

	playerVictim = GameView::instance().findPlayerByPersonaje(victim);
	playerAttacker = GameView::instance().findPlayerByPersonaje(attacker);
	if ((playerAttacker == NULL) || (playerVictim == NULL)) {
		return;
	}
	attackScore = playerAttacker->misionScore();
	victimScore = playerVictim->misionScore();
	playerAttacker->misionScore(attackScore + KILL);
	playerVictim->misionScore(victimScore - DEATH);
}

bool DeathmatchMision::missionEnded() {
	Player* player;

	player = GameView::instance().playerWithHighestScore();
	if (player->misionScore() >= MAX_SCORE) {
		return true;
	}
	return false;
}