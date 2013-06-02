#pragma once
#include "Item.h"
class Lamp : public Item
{
public:
	Lamp(void);
	~Lamp(void);
	void modifyCharacter(PersonajeModelo* personaje);
};

