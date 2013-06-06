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
Item* ItemFactory::createItem(float porcentajeAparicion,std::pair <int,int> pos)
{
	if(porcentajeAparicion>0)
	{
	int indice=static_cast <int> ((100.0/porcentajeAparicion) * NUMBERITEMS);
	int random= rand() % indice;
	switch (random){
		case 0:
			return new Lamp("Lamp",pos);
		case 1:
			return new RunningBoots("RunningBoots",pos);
		case 2:
			return new MapItem("MapItem",pos);
		default:
			return NULL;
	}
}

}
