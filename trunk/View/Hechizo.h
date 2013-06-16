#pragma once

#ifndef HECHIZO_H
#define HECHIZO_H

#include <string.h>
#include <string>

class Hechizo {

public:
	Hechizo(float coste, std::string spellId);
	~Hechizo();
	//No todos los metodos son necesarios para todos los hechizos
	virtual bool startSpell(std::string actualCaster);

protected:
	float cost;
	std::string spellId;
};

#endif