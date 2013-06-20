#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include "GameDependent.h"
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
	EXPLOSIVE_INACTIVE,
	EXPLOSIVE_FLYING,
	EXPLOSIVE_EXPLOSION_COUNTDOWN,
	EXPLOSIVE_BOUNCING,
	EXPLOSIVE_BURNING,
	EXPLOSIVE_EXPLOSION,
	EXPLOSIVE_DUST_IN_THE_WIND,
	ITEM_WAITING_REGENERATION

};



class Entity : public GameDependent {

protected:
	SDL_Rect spriteRect;
	Sprite * sprite;
	SDL_Rect posicionIsometricaPorTiles(int tileX,int tileY,Sprite* sprite);
	bool fogged;
	float endStatusTime;
	int freezedSpriteState;
	unsigned status;
	std::string name;

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
	void setEndStatusTime(float endTime);
	void decreaseEndStatusTime();
	void iceUp(float seconds);
	virtual bool isItem();
	bool needsCountDown();
	bool needsToBeCleaned();
	std::string statusToString();
	unsigned getTileWidth();
	unsigned getTileHeight();
	void setSprite(Sprite* sprite);
	void updateRectanglePosition(int x, int y);
	virtual bool isAlive();
	Sprite* getSprite();
	std::string getName();
	void setName(std::string value);
};

#endif // _ENTITY_H_