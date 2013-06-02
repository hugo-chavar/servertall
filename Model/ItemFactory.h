#pragma once
#include "Lamp.h"
#include "MapItem.h"
#include "RunningBoots.h"

class ItemFactory
{

public:
	ItemFactory(void);
	~ItemFactory(void);
	Item* createItem(float porcentajeAparicion);
};

