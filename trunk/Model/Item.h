#pragma once
#include "PersonajeModelo.h"
class Item
{
protected:
	bool alive;
	bool hidden;

public:
	Item(void);
	~Item(void);
	virtual void modifyCharacter(PersonajeModelo* personaje);
};

