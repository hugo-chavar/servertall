#pragma once
#include "Item.h"
#define FACTORVEL 1.5 //factor de aumento del rango de velocidad


class RunningBoots : public Item
{
public:
	RunningBoots(string _name,std::pair <int,int> _pos);
	~RunningBoots(void);
	void modifyCharacter(PersonajeModelo* personaje);
};

