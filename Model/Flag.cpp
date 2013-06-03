#include "Flag.h"
#include "Game.h"


Flag::Flag(int xPosition, int yPosition) {
	_life = 100; // HARCODEADO
	entity = Game::instance().yParser.findAnimatedEntityType("bandera");
	position.first = xPosition;
	position.second = yPosition;
}

Flag::~Flag() { }

void Flag::hurt(float damage) {
	if (damage > this->_life)
		_life = 0;
	else
		_life -= damage;
}

float Flag::life() {
	return _life;
}