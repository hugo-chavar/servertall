#include "Flag.h"
#include "GameView.h"


Flag::Flag() {
	vidaMaxima = 100; // HARCODEADO
	vidaActual = vidaMaxima;
	entity =  Game::instance().yParser.findAnimatedEntityType("bandera");
	bool validPosition = false;
	KeyPair position;
	while (!validPosition) {
		position.first = rand() % Game::instance().world()->width();
		position.second = rand() % Game::instance().world()->height();
		if ((!GameView::instance().getWorldView()->getTileAt(position)->hasOtherEntity()) && (!GameView::instance().getWorldView()->getTileAt(position)->getRelatedTile()))
			validPosition = true;
	}
	_position.first = 0;//position.first;
	_position.second = 0;//position.second;
	GameView::instance().getWorldView()->addOtherEntity(_position, entity->name());
	Game::instance().world()->getTileAt(_position)->setOtherEntity(entity);
}

Flag::~Flag() { }

string Flag::getName() {
	return entity->name();
}

pair <int,int> Flag::position() {
	return _position;
}

void Flag::destroy() {
	GameView::instance().getWorldView()->removeOtherEntity(_position);
	Game::instance().world()->getTileAt(_position)->setOtherEntity(NULL);
}