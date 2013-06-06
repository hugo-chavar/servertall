#pragma once
#include "Item.h"
class Lamp : public Item
{
public:
	Lamp(string _name,std::pair <int,int> _pos);
	~Lamp(void);
	void modifyCharacter(PersonajeModelo* personaje);
};

