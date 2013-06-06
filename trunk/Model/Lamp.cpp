#include "Lamp.h"
#define FACTORVISION 1.5 //factor de aumento del rango de vision

Lamp::Lamp(string _name,std::pair <int,int> _pos):Item(_name,_pos)
{
}


Lamp::~Lamp(void)
{
}

void Lamp::modifyCharacter(PersonajeModelo* personaje)
{
	personaje->increaseVisionRange(FACTORVISION);
}