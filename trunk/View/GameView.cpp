#include "GameView.h"
#include "Daniable.h"


GameView::GameView() { }

GameView::~GameView() {
	if (errorImage != NULL) {
		delete errorImage;
	}
}

bool GameView::initialize() {
	bool modelInitialized = Game::instance().initialize();;

	bool mapInitialized = this->worldView.initialize();

	// ELEGIR MISIÓN ANTES
	mision.initialize();

	this->getTimer()->initializeTime();

	AnimatedEntity errorEntity;
	
	errorEntity.loadImages(ERROR_ANIMATED_DIR);

	errorEntity.delay(0);

	errorImage = new SpriteAnimado(&errorEntity);

	return (modelInitialized&mapInitialized);
}

GameView& GameView::instance() {
	static GameView singleton;
	return singleton;
}

Stage* GameView::getWorldView() {
	return &worldView;
}

//Personaje* GameView::getPersonaje(string name) {
//	Personaje* personaje = NULL;
//	std::map<string,Personaje*>::iterator it = this->personajes.find(name);
//
//	if (it != this->personajes.end())
//		personaje = it->second;
//
//	return personaje;
//}

void GameView::addPlayer(string userID, string characterType) {
	Player *player = findPlayer(userID);
	if(player==NULL)
	{
	Personaje* character =characterFactory.createViewCharacter(characterType, userID) ;
	//Personaje* character = new Personaje(Game::instance().world()->vMainCharacters()->at(stringUtilities::stringToInt(characterType)));
	Player *player = new Player(userID, character);
	//player->initialize(Game::instance().configuration()->mainCharacterSpeed(), userID, Game::instance().configuration()->visionRange());
	this->_players.push_back(player);
	}
	else
	{
		player->getCharacter()->personajeModelo()->setActive(true);
	}
}

Player* GameView::findPlayer(string userID) {
	bool found = false;
	Player* player = NULL;
	unsigned i = 0;
	while ((i < this->_players.size()) && (!found)) {
		if (this->_players[i]->getUserID() == userID) {
			found = true;
			player = this->_players[i];
		}
		else
			i++;
	}
	return player;
}

//DONE: Changed return value to void
void GameView::manageMovementUpdate(string userID, string destination) { //, unsigned int deltaTime
	Player *player = findPlayer(userID);
	/*pair <int, int> current = player->getCharacter()->personajeModelo()->getPosition();*/
	pair <int, int> pair_destination = stringUtilities::stringToPairInt(destination);
	player->getCharacter()->setDestino(pair_destination.first, pair_destination.second);
	/*bool finished = false;
	int numberOfTiles = 0;*/
	/*string movementArgument = "";
	string str_nextTiles = "";*/
	//while (!finished) {
	//	//player->getCharacter()->mover(deltaTime);
	//	/*if (nextTile.first<0) {
	//		numberOfTiles = 0;
	//		movementArgument = stringUtilities::intToString(numberOfTiles);
	//		return movementArgument;
	//	}*/


	//	// SACADO PARA QUE COMPILE

	//	//str_nextTiles = str_nextTiles+","+stringUtilities::pairIntToString(nextTile);
	//	//player->getCharacter()->setCurrent(nextTile.first, nextTile.second);
	//	//if ((nextTile.first==pair_destination.first) && (nextTile.second==pair_destination.second))
	//	//	finished = true;

	//	numberOfTiles++;
	//}
	/*movementArgument = stringUtilities::intToString(numberOfTiles);
	movementArgument = movementArgument+str_nextTiles;*/
	/*player->getCharacter()->personajeModelo()->setCurrent(current.first, current.second);*/
	//int numberOfTiles = 1;
	//string movementArgument = stringUtilities::intToString(numberOfTiles);
	//pair <int, int> pair_destination = stringUtilities::stringToPairInt(destination);
	//player->getCharacter()->setDestino(pair_destination.first, pair_destination.second);
	///*pair <int, int> nextTile = player->getCharacter()->mover(pair_destination);
	//if (nextTile.first<0) {
	//	numberOfTiles = 0;
	//	movementArgument = stringUtilities::intToString(numberOfTiles);
	//	return movementArgument;
	//}*/
	/*player->getCharacter()->setCurrent(nextTile.first, nextTile.second);*/
	////string str_nextTile = stringUtilities::pairIntToString(nextTile);
	////movementArgument = movementArgument+","+str_nextTile;
	/*return movementArgument;*/
}

void GameView::manageAnimationUpdate(string userID, string animation) {
	char opcion_animacion;
	Player *player = findPlayer(userID);
	if (animation.size() == 1) {
		opcion_animacion = animation.front();
		player->getCharacter()->animateModel(opcion_animacion);
	}
}

Daniable* GameView::getDaniableInTile(std::pair <int, int> tile) {
	
	if(Game::instance().world()->isInsideWorld(tile))
	{
		vector <Player*>::iterator it;
		for(it=this->_players.begin(); it!=this->_players.end(); it++)
		{
			pair<int,int> posChar = (*it)->getCharacter()->getPosicionEnTiles();
			if(posChar == tile)
			{
				return (*it)->getCharacter();
			}
		}
	}
	return NULL;
}

//bool GameView::isThereACharInTile(int tileX, int tileY) {
//	pair<int,int> tilePos (tileX, tileY);
//	if(Game::instance().world()->isInsideWorld(tilePos))
//	{
//		vector <Player*>::iterator it;
//		for(it=this->_players.begin(); it!=this->_players.end(); it++)
//		{
//			pair<int,int> posChar = (*it)->getCharacter()->getPosicionEnTiles();
//			if(posChar == tilePos)
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}

string GameView::managePlayersUpdate() {
	string argument = "";
	if (_players.size() > 0) {
		for (unsigned i = 0; i < _players.size(); i++) {
			if (_players[i]->isUpdating())
				argument.append(_players[i]->getUserID() + ";" + _players[i]->getCharacter()->updateToString() + ":");
		}
		if (argument.size() > 0)
		{
			//argument.append(stringUtilities::intToString(static_cast <int> (SDL_GetTicks())));
			argument.pop_back();
		}
	}
	return argument;
}

string GameView::managePlayerInitialSynch(string userID) {
	Player *player = findPlayer(userID);
	string characterInit="";
	if (player)
		characterInit = player->getCharacter()->initToString();
	return characterInit;
}

bool GameView::isCharacterTypeValid(string characterType) {
	int type = stringUtilities::stringToInt(characterType);
	int charactersCount = static_cast< int > (Game::instance().world()->vMainCharacters()->size());
	if (( charactersCount >= type) && (type >= 0))
		return true;
	return false;
}

void GameView::update() {
	for(unsigned i = 0; i < this->_players.size(); i++) {
		if (_players[i]->isUpdating())
			_players[i]->getCharacter()->update();
	}
	this->getTimer()->updateTime();
	if (mision.isGameOver()) {
		Player* player = this->playerWithHighestScore();
		// TERMINAR JUEGO
	}
}

void GameView::startUpdatingPlayer(string userID) {
	Player *player = findPlayer(userID);
	player->startUpdating();
}

TimeManager* GameView::getTimer() {
	return &time;
}

void GameView::setDisconnectedPlayer(string userID) {
	Player *player = findPlayer(userID);
	player->getCharacter()->personajeModelo()->setActive(false);
}

//void GameView::wakeUpPlayer(string userID) {
//	Player *player = findPlayer(userID);
//	player->getCharacter()->personajeModelo()->setActive(true);
//}

SpriteAnimado* GameView::getErrorImage() {
	return errorImage;
}

string GameView::manageCharactersPlaying() {
	string argument = "";
	if (_players.size() > 0) {
		for (unsigned i = 0; i < _players.size(); i++) {
			if (_players[i]->isUpdating())
				argument.append( _players[i]->getCharacter()->idToString() + ":");
		}
		if (argument.size() > 0)
			argument.pop_back();
	}
	return argument;
}

Player* GameView::findPlayerByPersonaje(Daniable* personaje) {
	Player* player = NULL;
	for (unsigned int i=0; i<_players.size(); i++) {
		if ((_players[i]->getCharacter()) == personaje) {
			player = _players[i];
			break;
		}					
	}
	return player;
}

Player* GameView::playerWithHighestScore() {
	int highestScore = 0;
	Player* player = NULL;
	for (unsigned int i=0; i<_players.size(); i++) {
		if (_players[i]->misionScore() > highestScore) {
			highestScore = _players[i]->misionScore();
			player = _players[i];
		}					
	}
	return player;
}

Mision* GameView::getMision() {
	return &mision;
}

//------------------------ METODOS QUE NO SE USAN EN UN PRINCIPIO EN EL SERVER-----------------------------

bool GameView::insidePlayerVision(Player player, std::pair<int,int> pos) {
	bool inside = player.getCharacter()->personajeModelo()->getVision()->isInsideVision(pos);

	if (!inside) {
		TileModel* relatedTile = Game::instance().world()->getTileAt(pos)->getRelatedTile();
		if (relatedTile) {//TODO: mejorar para optimizar codigo
			// preguntar si es drawable() e ir salteando..
			while ( (!inside) && (relatedTile != Game::instance().world()->getTileAt(pos)) ) {
				pair<int, int> posRelated = relatedTile->getPosition();
				inside = player.getCharacter()->personajeModelo()->getVision()->isInsideVision(posRelated);
				relatedTile = relatedTile->getRelatedTile();
			}
		}
	}
	return inside;
}

bool GameView::isKnownByPlayer(Player player, std::pair<int,int> pos) {
	return player.getCharacter()->personajeModelo()->getVision()->testPosition(pos);
}


