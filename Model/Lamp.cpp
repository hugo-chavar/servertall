#include "Lamp.h"
#define FACTORVISION 1.5 //factor de aumento del rango de vision

Lamp::Lamp(void)
{
}


Lamp::~Lamp(void)
{
}

void Lamp::modifyCharacter(PersonajeModelo* personaje)
{
	personaje->increaseVisionRange(FACTORVISION);
}