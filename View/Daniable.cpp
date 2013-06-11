#include "Daniable.h"

Daniable::Daniable()
{
	vidaActual = 0;
}

Daniable::~Daniable() { }

void Daniable::recibirDano(float dano) {
	if (dano > this->vidaActual)
		vidaActual = 0;
	else
		vidaActual -= dano;
}
//
//std::pair<int, int> Daniable::getPosicionActualEnTiles() {
//	return tileActual;
//}

bool Daniable::isAlive() {
	if (vidaActual > 0) {
		return true;
	}
	return false;
}