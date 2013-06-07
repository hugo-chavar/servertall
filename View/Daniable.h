#pragma once


class Daniable
{
public:
	Daniable(void);
	~Daniable(void);
	virtual void recibirDano(float dano);
	bool isAlive();

protected:
	float vidaMaxima;
	float vidaActual;
};

