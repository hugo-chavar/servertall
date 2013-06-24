#include "IceBomb.h"
#include "GameView.h"

IceBomb::IceBomb() {
	this->setName("IceBomb");
	this->setAmmunitionType(BOMB);
	this->setStatus(EXPLOSIVE_INACTIVE);
	explosionSprite = (SpriteAnimado*)(this->getSpriteWithName("blueexplosion"));
	this->setNeedsUpdate(false);
}

IceBomb::~IceBomb() {
}

bool IceBomb::isAlive() {
	return (this->getStatus() != EXPLOSIVE_DUST_IN_THE_WIND);
}

void IceBomb::update() {
	switch (this->getStatus()){
		case EXPLOSIVE_EXPLOSION_COUNTDOWN: {
			this->decreaseEndStatusTime();
			if (this->endStatusTime == 0)
				this->setStatus(EXPLOSIVE_EXPLOSION);
			break;
		}
		case EXPLOSIVE_EXPLOSION: {
			this->setStatus(EXPLOSIVE_BURNING);
			explosionSprite->restart();
			explosionSprite->advance();
			explosionSprite->setAccumulatedTime(0.0);
			this->setCenter(this->getPosition());
			this->setRange(4);
			this->activate();
			this->range.fill();
			std::pair<int, int > aux;
			while (this->range.hasNext()) {
				aux = this->range.next();
				if (aux != this->getPosition() ) {
					Daniable* daniable = GameView::instance().getDaniableInTile(aux);
					if (daniable) {
						daniable->iceUp(5);
					}
				}
			}
			this->setNeedsUpdate(true);
			break;
		}
		case EXPLOSIVE_BURNING: {
			
			explosionSprite->updateFrame();
			this->decreaseEndStatusTime();
			if ((this->endStatusTime == 0)&&(explosionSprite->lastFrame())) {
				this->setStatus(EXPLOSIVE_DUST_IN_THE_WIND);
				this->setAvailable(true);
			}
			break;
		}
	}
}
		
void IceBomb::startCountDown(float seconds) {
	this->setEndStatusTime(seconds);
}

void IceBomb::deserialize(std::string in) {
	std::vector<std::string> splittedIn;
	stringUtilities::splitString(in,splittedIn,'?');

	this->setName(splittedIn[0]);
	this->setAmmoID(splittedIn[1]);
	this->positionFromString(splittedIn[2]);
}

std::string IceBomb::serialize() {
	std::string out = this->getName();
	out.append("?");
	out.append(this->getAmmoId());
	out.append("?");
	out.append(this->positionToString());
	out.append("?");
	out.append(this->statusToString());
	return out;
}

bool IceBomb::needsUpdates() {
	if(!this->needsUpdate)
		return false;
	this->needsUpdate=false;
	return true;
}

void IceBomb::setNeedsUpdate(bool needs) {
	this->needsUpdate=needs;
}