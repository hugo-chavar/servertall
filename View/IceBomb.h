#pragma once

#include "Movable.h"
#include "Daniable.h"
#include "Explosive.h"
#include "SpriteAnimado.h"
#include "../DedicatedServer/Serializable.h"

class IceBomb: public Entity, public Explosive, public Positionable, public Serializable {

private:
	SpriteAnimado* explosionSprite;
	bool needsUpdate;

public:

	IceBomb();
	~IceBomb();
	bool isAlive();
	void update();
	void startCountDown(float seconds);
	void deserialize(std::string in);
	std::string serialize();
	bool needsUpdates();
	void setNeedsUpdate(bool needs);
};

