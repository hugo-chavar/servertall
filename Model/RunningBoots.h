#pragma once
#include "Item.h"
#define FACTORVEL 1.5 //factor de aumento del rango de velocidad


class RunningBoots : public Item
{
public:
	RunningBoots(void);
	~RunningBoots(void);
	void modifyCharacter(PersonajeModelo* personaje);
};

