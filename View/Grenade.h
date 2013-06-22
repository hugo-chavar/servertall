#ifndef _GRENADE_H_
#define _GRENADE_H_

#include "Movable.h"
#include "Daniable.h"
#include "Explosive.h"
#include "SpriteAnimado.h"

class Grenade: public Movable, public Explosive {

private:
	SpriteAnimado * explosionSprite;
	bool needsUpdate;

public:
	Grenade();
	~Grenade();
	bool isAlive();
	void update();
	void startCountDown(float seconds);
	bool needsUpdates();
	void setNeedsUpdate(bool needs);
};


#endif // _GRENADE_H_