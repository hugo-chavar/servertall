#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include "Sprite.h"



enum entityStatus_t {
	HIDDEN_ITEM,
	UNCOVER_ITEM,
	DEATH_ITEM,
	REVIVE_UNCOVER_ITEM,
	REVIVE_HIDDEN_ITEM,
	ENTITY_FROZEN,
	ENTITY_BLINKING,
	ENTITY_NORMAL,
	EXPLOSIVE_AVAILABLE,
	EXPLOSIVE_EXPLOSION_COUNTDOWN,
	EXPLOSIVE_EXPLOSION,
	EXPLOSIVE_DUST_IN_THE_WIND,
	ITEM_WAITING_REGENERATION

};



class Entity
{
protected:
	SDL_Rect spriteRect;
	Sprite * sprite;
	SDL_Rect posicionIsometricaPorTiles(int tileX,int tileY,Sprite* sprite);
	bool fogged;
	Uint32 endStatusTime;
	int freezedSpriteState;
	unsigned status;

public:
	Entity();
	Entity(int tileX,int tileY,Sprite* spriteCargado);
	virtual void update();
	~Entity(void);
	void setFogged(bool);
	bool isFogged();
	void setRectangle(std::pair<int, int> pos, Sprite* sprite );
	void resetSpriteState();
	void setStatus(unsigned status);
	unsigned getStatus();
	bool isImmobilized();
	void setEndStatusTime(Uint32 endTime);
	void decreaseEndStatusTime(float timeToDecrease);
	void iceUp(unsigned seconds);
	virtual bool isItem();
	bool needsCountDown();
	bool needsToBeCleaned();
	string statusToString();
};

#endif // _ENTITY_H_