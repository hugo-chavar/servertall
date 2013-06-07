#include "Daniable.h"


Daniable::Daniable(void)
{
	vidaMaxima = 0;
	vidaActual = 0;
}


Daniable::~Daniable(void)
{
}

void Daniable::recibirDano(float dano)
{
	//ABSTRACT METHOD
}

bool Daniable::isAlive() {
	if (vidaActual > 0) {
		return true;
	} else {
		return false;
	}
	return true;
}