#include "MapItem.h"


MapItem::MapItem(void)
{
}


MapItem::~MapItem(void)
{
}

void MapItem::modifyCharacter(PersonajeModelo* personaje)
{
	personaje->discoverMap();
}
