#include "Flag.h"
#include "GameView.h"


Flag::Flag() {
	int vidaMaxima = 100; // HARCODEADO
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
	tileActual.first = 4;//position.first;
	tileActual.second = 3;//position.second;
	GameView::instance().getWorldView()->addOtherEntity(tileActual, entity->name());
	Game::instance().world()->getTileAt(tileActual)->setOtherEntity(entity);
}

Flag::~Flag() { }

string Flag::getName() {
	return entity->name();
}

void Flag::destroy() {
	GameView::instance().getWorldView()->removeOtherEntity(tileActual);
	Game::instance().world()->getTileAt(tileActual)->setOtherEntity(NULL);
}