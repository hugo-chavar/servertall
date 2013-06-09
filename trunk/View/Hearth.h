#pragma once

#ifndef HEARTH_H
#define HEARTH_H

#include "ItemView.h"

class Hearth : public ItemView {

public:
	Hearth(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite);
	~Hearth(void);
	void modifyCharacter(Personaje* personaje);
};

#endif