#ifndef DANIABLE_H
#define DANIABLE_H

#include <utility>

class Daniable
{
public:
	Daniable(void);
	~Daniable(void);
	virtual void recibirDano(float dano);
	virtual std::pair<int, int> getPosicionActualEnTiles();
	bool isAlive();

protected:
	float vidaMaxima;
	float vidaActual;
};

#endif

