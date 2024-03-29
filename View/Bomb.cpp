#include "GameView.h"
#include "Bomb.h"

Bomb::Bomb() {
	this->setName("Bomb");
	this->setAmmunitionType(BOMB);
	this->setStatus(EXPLOSIVE_INACTIVE);
	this->setSprite(this->getSpriteWithName(this->getName()));
	explosionSprite = (SpriteAnimado*)(this->getSpriteWithName("redexplosion"));
	this->setNeedsUpdate(false);
}

Bomb::~Bomb() {
}

bool Bomb::isAlive() {
	return (this->getStatus() != EXPLOSIVE_DUST_IN_THE_WIND);
}

void Bomb::update() {
	switch (this->getStatus()){
		case EXPLOSIVE_EXPLOSION_COUNTDOWN: {
			this->decreaseEndStatusTime();
			if (this->endStatusTime == 0)
				this->setStatus(EXPLOSIVE_EXPLOSION);
			break;
		}
		case EXPLOSIVE_EXPLOSION: {
			this->setStatus(EXPLOSIVE_BURNING);
			explosionSprite->setAccumulatedTime(0.0);
			explosionSprite->restart();
			this->setCenter(this->getPosition());
			this->setRange(3);
			this->activate();
			this->range.fill();
			std::pair<int, int > aux;
			while (this->range.hasNext()) {
				aux = this->range.next();
				Daniable* daniable = GameView::instance().getDaniableInTile(aux);
				if ((daniable) && (daniable->isAlive())) {
					daniable->recibirDano(this->getDamage());
					if (!(daniable->isAlive()))
						GameView::instance().getMission()->missionUpdate(daniable, this->getOwner());
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
		
void Bomb::startCountDown(float seconds) {
	this->setEndStatusTime(seconds);
}

void Bomb::deserialize(std::string in) {
	std::vector<std::string> splittedIn;
	stringUtilities::splitString(in,splittedIn,'?');

	this->setName(splittedIn[0]);
	this->setAmmoID(splittedIn[1]);
	this->positionFromString(splittedIn[2]);
}

std::string Bomb::serialize() {
	std::string out = this->getName();
	out.append("?");
	out.append(this->getAmmoId());
	out.append("?");
	out.append(this->positionToString());
	out.append("?");
	out.append(this->statusToString());
	return out;
}

bool Bomb::needsUpdates() {
	if(!this->needsUpdate)
		return false;
	this->needsUpdate=false;
	return true;
}

void Bomb::setNeedsUpdate(bool needs) {
	this->needsUpdate=needs;
}