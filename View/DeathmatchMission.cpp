#include "DeathmatchMission.h"
#include "GameView.h"


#define KILL 3
#define DEATH 2
#define MAX_SCORE 3


DeathmatchMission::DeathmatchMission() {
	chosen = false;
	winner = "";
}

DeathmatchMission::~DeathmatchMission() { }

void DeathmatchMission::initialize() {
	this->winner = "";
	GameView::instance().restartPlayersScores();
}

void DeathmatchMission::choose() {
	chosen = true;
}

bool DeathmatchMission::isTheChosenMission() {
	return chosen;
}

void DeathmatchMission::updateMissionStatus(Daniable* victim, string attacker) {
	Player* playerVictim = NULL;
	Player* playerAttacker = NULL;
	int attackScore = 0;
	int victimScore = 0;

	playerVictim = GameView::instance().findPlayerByPersonaje(victim);
	playerAttacker = GameView::instance().findPlayer(attacker);
	if ((playerAttacker == NULL) || (playerVictim == NULL)) {
		return;
	}
	attackScore = playerAttacker->missionScore();
	victimScore = playerVictim->missionScore();
	playerAttacker->missionScore(attackScore + KILL);
	playerVictim->missionScore(victimScore - DEATH);
}

bool DeathmatchMission::missionEnded() {
	Player* player;
	player = GameView::instance().playerWithHighestScore();
	if (player == NULL)
		return false;
	if (player->missionScore() >= MAX_SCORE) {
		winner = player->getUserID();
		return true;
	}
	return false;
}

string DeathmatchMission::getWinner() {
	return this->winner;
}