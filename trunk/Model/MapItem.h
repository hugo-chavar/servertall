#pragma once
#include "Item.h"
class MapItem : public Item
{
public:
	MapItem(void);
	~MapItem(void);
	void modifyCharacter(PersonajeModelo* personaje);
};

