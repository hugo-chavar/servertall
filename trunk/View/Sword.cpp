#include "Game.h"
#include "GameView.h"
#include "Sword.h"

Sword::Sword() {
	this->setAmmo(1);
}

Sword::~Sword() {
}

void Sword::strike(Daniable* target) {
	if (target->isAlive()) {
		float precision = Game::instance().getRandom();
		if (precision >= this->getPrecision()) {
			target->recibirDano(this->getDamage());
			if(target->isWood())
				GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ATTACK_ON_WOOD)+";"+this->positionToString());
			else
				GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ATTACK_ON_SHIELD)+";"+this->positionToString());
			if (!(target->isAlive()))
				GameView::instance().getMission()->missionUpdate(target, this->getOwner());
		}
	}	
}