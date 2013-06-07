#pragma once
#include "ItemView.h"
class Lamp : public ItemView
{
public:
	Lamp(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite);
	~Lamp(void);
	void modifyCharacter(Personaje* personaje);
};

