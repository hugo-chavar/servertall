#include "MapItem.h"


MapItem::MapItem(string _name,std::pair <int,int> _pos):Item(_name,_pos)
{
}


MapItem::~MapItem(void)
{
}

void MapItem::modifyCharacter(PersonajeModelo* personaje)
{
	personaje->discoverMap();
}
