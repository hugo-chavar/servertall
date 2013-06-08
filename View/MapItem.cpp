#include "MapItem.h"


MapItem::MapItem(string _name,string _hidden,std::pair <int,int> _pos,Sprite* _hiddenSprite, Sprite* sprite) : ItemView(_name,_hidden,_pos,_hiddenSprite,sprite)
{
}


MapItem::~MapItem(void)
{
}

void MapItem::modifyCharacter(Personaje* personaje)
{
	personaje->personajeModelo()->getVision()->setAllKnown(true);
}
