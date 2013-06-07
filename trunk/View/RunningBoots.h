#pragma once
#include "ItemView.h"
#define FACTORVEL 1.5 //factor de aumento del rango de velocidad

class RunningBoots : public ItemView
{
public:
	RunningBoots(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite);
	~RunningBoots(void);
	void modifyCharacter(Personaje* personaje);
};

