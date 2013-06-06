#include "RunningBoots.h"


RunningBoots::RunningBoots(string _name,std::pair <int,int> _pos):Item(_name,_pos)
{
}


RunningBoots::~RunningBoots(void)
{
}

void RunningBoots::modifyCharacter(PersonajeModelo* personaje)
{
	personaje->increaseSpeed(FACTORVEL);
}