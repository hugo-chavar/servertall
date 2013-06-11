#ifndef _AMMUNITION_H_
#define _AMMUNITION_H_

#include "Entity.h"
#include "Personaje.h"
#include "StringUtilities.h"
#include "Daniable.h"

class Ammunition : public Entity
{

protected:
	unsigned state;
	//bool alive;
	//bool hidden;
	string name;
	string itemChangeToString(unsigned _state);
	SDL_Rect hiddenSpriteRect;
	Sprite * hiddenSprite;
	void setHiddenRectangle(std::pair<int, int> pos, Sprite* sprite );
	int regenerationTime;
	bool canReviveForHimself;

public:
	void update();
	Ammunition(string _name, Sprite* sprite);
	~Ammunition();
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
	//unsigned getState();
	bool getCanReviveForHimself();
};


#endif // _AMMUNITION_H_

