#pragma once
#include "PersonajeModelo.h"
#include "StringUtilities.h"
class Item
{
protected:
	bool alive;
	bool hidden;
	string name;
	std::pair<int,int> pos;
	string Item::itemChangeToString(char change);

public:
	Item(string _name,std::pair <int,int> _pos);
	~Item(void);
	virtual void modifyCharacter(PersonajeModelo* personaje);
	string kill();
	bool isAlive();
	bool isHidden();
	string uncover();
	string getName();
	std::pair<int,int> getPos();
	string revive();
};

