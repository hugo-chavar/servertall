#include "Item.h"

//clase abstracta
Item::Item(string _name,std::pair <int,int> _pos)
{
	this->name=_name;
	this->alive=true;
	this->hidden=false;//Lo hago asi para que cofres no bloqueen camino
	this->pos=_pos;
}


Item::~Item(void)
{
}

void Item::modifyCharacter(PersonajeModelo* personaje)
{
	//metodo abstracto
}

string Item::kill()
{
	this->alive=false;
	return	itemChangeToString('D');
}

string Item::revive()
{
	this->alive=true;
	return(itemChangeToString('A'));
}

bool Item::isAlive()
{
	return this->alive;
}

bool Item::isHidden()
{
	return this->hidden;
}

string Item::uncover()
{
	this->hidden=false;
	return itemChangeToString('H');
}

string Item::getName()
{
	return this->name;
}

std::pair<int,int> Item::getPos()
{
	return pos;
}

string Item::itemChangeToString(char change)
{
	string modification=stringUtilities::pairIntToString(this->getPos())+","+change;
	return modification;
}