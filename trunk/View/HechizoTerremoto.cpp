#include "HechizoTerremoto.h"
#include "Personaje.h"
#include "GameView.h"

HechizoTerremoto::HechizoTerremoto(float coste, std::string spellId) : Hechizo(coste, spellId)
{
}


HechizoTerremoto::~HechizoTerremoto(void)
{
}

bool HechizoTerremoto::startSpell(std::string actualCaster) {
	Personaje* invocador = NULL;
	invocador = GameView::instance().findPlayer(actualCaster)->getCharacter();
	if (invocador == NULL) {
		return false;
	}
	if (invocador->useMagic(cost)) {
		//Realizar el terremoto
		GameView::instance().addEventUpdate("vibrar");
		return true;
	}
	return false;
}