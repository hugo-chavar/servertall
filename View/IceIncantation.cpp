#include "GameView.h"
#include "IceIncantation.h"

IceIncantation::IceIncantation():ImpactAmmo() {
	this->setName("IceIncantation");
	this->setAmmunitionType(IMPACT_AMMO);
	//this->setDamage(50.0);
	this->setSprite(this->getSpriteWithName(this->getName()));
}

IceIncantation::~IceIncantation() {
}

void IceIncantation::impact(Daniable* daniable) {
	if (daniable) {
		if (daniable->getPosition() == this->getInitialTile())
			return;
		GameView::instance().addEventUpdate(stringUtilities::intToString(EVENT_SOUND_ICESPELL_IMPACT)+";"+stringUtilities::pairIntToString(this->getCurrentTile()));
		daniable->iceUp(5);
		this->setTargetReached(true);
		this->setAvailable(true);
	}
}
