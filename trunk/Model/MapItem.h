#pragma once
#include "Item.h"
class MapItem : public Item
{
public:
	MapItem(string _name,std::pair <int,int> _pos);
	~MapItem(void);
	void modifyCharacter(PersonajeModelo* personaje);
};

