#include "Flag.h"
#include "GameView.h"


Flag::Flag() {
	_life = 100; // HARCODEADO
	entity = Game::instance().yParser.findAnimatedEntityType("bandera");
	bool validPosition = false;
	KeyPair position;
	while (!validPosition) {
		position.first = rand() % Game::instance().world()->width();
		position.second = rand() % Game::instance().world()->height();
		if ((!GameView::instance().getWorldView()->getTileAt(position)->hasOtherEntity()) && (!GameView::instance().getWorldView()->getTileAt(position)->getRelatedTile()))
			validPosition = true;
	}
	_position.first = position.first;
	_position.second = position.second;
	// TODO: Agregar entidad al mapa.
	//GameView::instance().getWorldView()->getTileAt(position)->createOtherEntity(entity);
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