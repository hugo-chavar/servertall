#include "ItemFactory.h"
#include <stdlib.h>

#define NUMBERITEMS 3 // Cantidad de items implementados

ItemFactory::ItemFactory(void)
{
}


ItemFactory::~ItemFactory(void)
{
}

//Porcentaje de aparicion indica el porcentaje de Tiles que tendran algun item
Item* ItemFactory::createItem(float porcentajeAparicion)
{
	int indice=(static_cast <int> (100.0/porcentajeAparicion)) * NUMBERITEMS;
	int random= rand() % indice;
	switch (random){
		case 0:
			return new Lamp();
		case 1:
			return new MapItem();
		case 2:
			return new MapItem();
		default:
			return NULL;
}

}
