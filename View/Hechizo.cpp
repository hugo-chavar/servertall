#include "Hechizo.h"


Hechizo::Hechizo(float coste, std::string spellId) {
	this->cost = coste;
	this->spellId = spellId;
}

bool Hechizo::startSpell(std::string actualCaster) {
	return true;
}

Hechizo::~Hechizo() {
}