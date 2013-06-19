#pragma once

#ifndef HECHIZOVIDENCIA_H
#define HECHIZOVIDENCIA_H

#include "Hechizo.h"

class HechizoVidencia : public Hechizo {

public:
	HechizoVidencia(float coste, std::string spellId);
	~HechizoVidencia();
	bool startSpell(std::string actualCaster);

};


#endif