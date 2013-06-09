#pragma once

#ifndef MAGICBOTTLE_H
#define MAGICBOTTLE_H

#include "ItemView.h"

class MagicBottle : public ItemView {

public:
	MagicBottle(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite);
	~MagicBottle(void);
	void modifyCharacter(Personaje* personaje);
};

#endif