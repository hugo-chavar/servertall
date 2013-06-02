#include "Item.h"

//clase abstracta
Item::Item(void)
{
	alive=true;
	hidden=true;
}


Item::~Item(void)
{
}

void Item::modifyCharacter(PersonajeModelo* personaje)
{
	//metodo abstracto
}
