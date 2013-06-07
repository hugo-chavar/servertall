#pragma once
#include "ItemView.h"
class MapItem : public ItemView
{
public:
	MapItem(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite);
	~MapItem(void);
	void modifyCharacter(Personaje* personaje);
};

