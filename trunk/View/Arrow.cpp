#include "GameView.h"
#include "Arrow.h"

Arrow::Arrow():ImpactAmmo() {
	this->setName("Arrow");
	this->setAmmunitionType(IMPACT_AMMO);
	//this->setDamage(50.0);
	this->setSprite(this->getSpriteWithName(this->getName()));
}

Arrow::~Arrow() {
}

void Arrow::impact(Daniable* daniable) {
	if (daniable) {
		if (daniable->getPosition() == this->getInitialTile())
			return;
		if(daniable->isWood())
			GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ATTACK_ON_WOOD)+";"+daniable->positionToString());
		else
			GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ATTACK_ON_SHIELD)+";"+daniable->positionToString());
		daniable->recibirDano(this->getDamage());
		this->setTargetReached(true);
		this->setAvailable(true);
		if (!(daniable->isAlive()))
			GameView::instance().getMission()->missionUpdate(daniable, this->getOwner());
	}
}