#include "GameView.h"
#include "Grenade.h"
#include "Logger.h"
#include "Daniable.h"
#include "../Common/StringUtilities.h"

Grenade::Grenade() {
	this->setName("Grenade");
	this->setAmmunitionType(HAND_GRENADE);
	this->setStatus(EXPLOSIVE_INACTIVE);
	this->setSprite(this->getSpriteWithName(this->getName()));
	explosionSprite = (SpriteAnimado*)this->getSpriteWithName("redexplosion");
}

Grenade::~Grenade() {
}

bool Grenade::isAlive() {
	return (this->getStatus() != EXPLOSIVE_DUST_IN_THE_WIND);
}

void Grenade::update() {
	switch (this->getStatus()){
		case EXPLOSIVE_FLYING: {
			Movable::verify();
			if ((!this->isTargetReached())&&(this->couldContinue()))
				Movable::update();
			else {
				this->setTargetTile(this->getLastTile());
				this->setStatus(EXPLOSIVE_BOUNCING);
				this->setTargetReached(false);
				this->setCouldContinue(true);
				this->changeToOppositeDirection();
			}
			break;
		}
		
		case EXPLOSIVE_BOUNCING: {
			int x, y;
			this->setCurrentTile(this->getCurrentTile() + this->getDirection());
			x = static_cast<int>(this->getTileWidth())*(this->getCurrentTile().first - this->getCurrentTile().second)/2 - this->getSprite()->relatx();
			y = static_cast<int>(this->getTileHeight())*(1 + this->getCurrentTile().first + this->getCurrentTile().second)/2 - this->getSprite()->relaty();
			this->setPosition(std::make_pair(x, y));
			this->setStatus(EXPLOSIVE_EXPLOSION_COUNTDOWN);
			this->startCountDown(5); //tarda 5 segundos en exlotar
		}
		break;
		case EXPLOSIVE_EXPLOSION_COUNTDOWN: {
			this->decreaseEndStatusTime();
			//common::Logger::instance().log("this->endStatusTime: " + stringUtilities::unsignedToString(static_cast<unsigned>(this->endStatusTime)));
			if (this->endStatusTime == 0) {
				this->setStatus(EXPLOSIVE_EXPLOSION);
				this->setNeedsUpdate(true);
			}
			break;
		}
		case EXPLOSIVE_EXPLOSION: {
			this->setStatus(EXPLOSIVE_BURNING);
			explosionSprite->setAccumulatedTime(0.0);
			explosionSprite->restart();
			this->setCenter(this->getCurrentTile());
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
				std::string data = this->serialize();
				std::string evento = stringUtilities::intToString(EVENT_AMMUNITION_CHANGE) + ";";
				GameView::instance().addEventUpdate(evento + data);
				this->setAvailable(true);
			}
			break;
		}
	}
}

std::string Grenade::serialize() {
	std::string out = this->getName();
	out.append("?");
	out.append(this->getAmmoId());
	out.append("?");
	out.append(this->positionToString());
	out.append("?");
	out.append(this->directionToString());
	out.append("?");
	out.append(this->statusToString());
	out.append("?");
	out.append(stringUtilities::pairIntToString(this->getTargetTile()));
	out.append("?");
	out.append(stringUtilities::pairIntToString(this->getCurrentTile()));
	out.append("?");
	out.append(stringUtilities::pairIntToString(this->getInitialTile()));
	out.append("?");
	out.append(stringUtilities::floatToString(this->getVelocity()));
	out.append("?");
	out.append(stringUtilities::pairIntToString(this->getLastTile()));
	out.append("?");
	if(this->couldContinue())
		out.append("T");
	else
		out.append("F");
	out.append("?");
	if(this->isTargetReached())
		out.append("T");
	else
		out.append("F");
	return out;
}

void Grenade::startCountDown(float seconds) {
	this->setEndStatusTime(seconds);
}

bool Grenade::needsUpdates() {
	if(!this->needsUpdate)
		return false;
	else if (this->status != EXPLOSIVE_FLYING)
		this->needsUpdate=false;
	return true;
}

void Grenade::setNeedsUpdate(bool needs)
{
	this->needsUpdate=needs;
}