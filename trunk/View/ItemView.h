#pragma once
#include "Entity.h"
#include "Personaje.h"
#include "StringUtilities.h"
#include "Daniable.h"

#ifndef item_state_t
#define item_state_t
enum item_state_t {
	HIDDEN_ITEM,
	UNCOVER_ITEM,
	DEATH_ITEM,
	REVIVE_UNCOVER_ITEM,
	REVIVE_HIDDEN_ITEM

};
#endif

class ItemView : public Entity,public Daniable
{

protected:
	unsigned state;
	//bool alive;
	//bool hidden;
	string name;
	string ItemView::itemChangeToString(unsigned _state);
	SDL_Rect hiddenSpriteRect;
	Sprite * hiddenSprite;
	void setHiddenRectangle(std::pair<int, int> pos, Sprite* sprite );
	int regenerationTime;
	bool canReviveForHimself;

public:
	void update();
	ItemView(string _name,unsigned _state,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite,bool _canReviveForHimself);
	~ItemView(void);
	virtual void modifyCharacter(Personaje* personaje);
	void kill();
	bool isAlive();
	bool isHidden();
	void uncover();
	string getName();
	//std::pair<int,int> getPos();
	void setPos(std::pair<int,int> position);
	void revive(unsigned _state,std::pair <int,int> _pos);
	void recibirDano(float dano);
	bool isItem();
	unsigned getState();
	bool getCanReviveForHimself();
};

