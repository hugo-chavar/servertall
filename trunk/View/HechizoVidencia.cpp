#include "HechizoVidencia.h"
#include "Personaje.h"
#include "GameView.h"



HechizoVidencia::HechizoVidencia(float coste, std::string spellId) : Hechizo(coste, spellId) {
}


HechizoVidencia::~HechizoVidencia() {
}

bool HechizoVidencia::startSpell(std::string actualCaster) {
	Personaje* invocador = NULL;
	invocador = GameView::instance().findPlayer(actualCaster)->getCharacter();
	if (invocador == NULL) {
		return false;
	}
	if (invocador->useMagic(cost)) {
		invocador->setVidencia(true);
		invocador->setVidCost(this->cost);
		invocador->setVidTime(0.0);
		return true;
	}
	return false;
}