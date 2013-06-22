#ifndef _BOMB_H_
#define _BOMB_H_

#include "Movable.h"
#include "Daniable.h"
#include "Explosive.h"
#include "../DedicatedServer/Serializable.h"

class Bomb: public Entity, public Explosive, public Positionable, public Serializable {

private:
	SpriteAnimado * explosionSprite;
	bool needsUpdate;
public:
	Bomb();
	~Bomb();
	bool isAlive();
	void update();
	void startCountDown(float seconds);
	void deserialize(std::string in);
	std::string serialize();
	bool needsUpdates();
	void setNeedsUpdate(bool needs);
};


#endif // _BOMB_H_