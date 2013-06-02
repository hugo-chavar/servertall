#include "RunningBoots.h"


RunningBoots::RunningBoots(void)
{
}


RunningBoots::~RunningBoots(void)
{
}

void RunningBoots::modifyCharacter(PersonajeModelo* personaje)
{
	personaje->increaseSpeed(FACTORVEL);
}