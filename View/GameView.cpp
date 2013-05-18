#include "GameView.h"


GameView::GameView() { }

GameView::~GameView() { }

bool GameView::initialize() //view::Camera camera
{
	//Yami este metodo todavia no esta.Hay que ver que cosas de aca van, porq es una copia del del modelo por ahora

	//	bool mapInitialized = worldView.initialize();
	//	bool chatInitialized = chat.initialize(camera);
	//	return (mapInitialized && chatInitialized);

	//YAMLParser connectionParser;
	//connectionParser.parse(CONNECTION_DIRECTORY, true);
	//int serverPortNumber = connectionParser.getConfigPort();
	//string serverIpAddress = connectionParser.getConfigIp();

	//ClientUpdater clientUpdater;
	//clientUpdater.setServerIp(serverIpAddress);
	//clientUpdater.setServerPort(serverPortNumber);
	////clientUpdater.updateClient();
	//yParser.parse(CONFIGFILE_DIRECTORY, false);
	//_world = yParser.vStages()[0];
	//unsigned stageActual = 0;
	//unsigned personActual = 0;
	//allEntities = yParser.allLists();
	//_configuration = yParser.getConfig();
	//_configuration.serverPort(serverPortNumber);
	//_configuration.serverIp(serverIpAddress);
	////selecciono el primero del primer stage
	//_personaje = yParser.modelMainCharacters(stageActual,personActual); 
	////si hubieron problemas salgo
	//if (!_personaje)
	//	return false;
	//_personaje->setVelocidad(_configuration.mainCharacterSpeed());
	//_personaje->createVision(_configuration.visionRange());
	//this->_personaje->setName(this->playerName);
	//personajes.insert(std::pair<string,PersonajeModelo*>(this->playerName,_personaje));
	//this->_time.initializeTime();
	//_login.initialize();
	//this->getModelUpdater()->startUpdating();
	return true;

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
	Personaje* character = new Personaje(Game::instance().world()->vMainCharacters()->at(stringUtilities::stringToInt(characterType)));
	Player *player = new Player(userID, character);
	player->getCharacter()->personajeModelo()->setVelocidad(Game::instance().configuration()->mainCharacterSpeed());
	player->getCharacter()->personajeModelo()->setName(userID);
	player->getCharacter()->personajeModelo()->createVision(Game::instance().configuration()->visionRange());
	this->_players.push_back(player);
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

string GameView::manageMovementUpdate(string userID, string destination) { //, unsigned int deltaTime
	Player *player = findPlayer(userID);
	pair <int, int> current = player->getCharacter()->personajeModelo()->getPosition();
	pair <int, int> pair_destination = stringUtilities::stringToPairInt(destination);
	player->getCharacter()->setDestino(pair_destination.first, pair_destination.second);
	bool finished = false;
	int numberOfTiles = 0;
	string movementArgument = "";
	string str_nextTiles = "";
	while (!finished) {
		//player->getCharacter()->mover(deltaTime);
		/*if (nextTile.first<0) {
			numberOfTiles = 0;
			movementArgument = stringUtilities::intToString(numberOfTiles);
			return movementArgument;
		}*/


		// SACADO PARA QUE COMPILE

		//str_nextTiles = str_nextTiles+","+stringUtilities::pairIntToString(nextTile);
		//player->getCharacter()->setCurrent(nextTile.first, nextTile.second);
		//if ((nextTile.first==pair_destination.first) && (nextTile.second==pair_destination.second))
		//	finished = true;

		numberOfTiles++;
	}
	movementArgument = stringUtilities::intToString(numberOfTiles);
	movementArgument = movementArgument+str_nextTiles;
	player->getCharacter()->personajeModelo()->setCurrent(current.first, current.second);
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
	return movementArgument;
}

string GameView::managePlayersUpdate() {
	string argument = "";
	if (_players.size() > 0) {
		for (int i=0; i < _players.size(); i++)
			argument = _players[i]->getUserID()+","+_players[i]->getCharacter()->toString()+":";
		argument.pop_back();
	}
	return argument;
}

string GameView::managePlayerInitialSynchPosition(string userID) {
	Player *player = findPlayer(userID);
	string position = stringUtilities::pairIntToString(player->getCharacter()->personajeModelo()->getPosition());
	return position;
}

string GameView::managePlayerInitialSynchVision(string userID) {
	Player *player = findPlayer(userID);
	string vision = player->getCharacter()->personajeModelo()->getVision()->toString();
	return vision;
}

bool GameView::isCharacterTypeValid(string characterType) {
	int type = stringUtilities::stringToInt(characterType);
	int charactersCount = static_cast< int > (Game::instance().world()->vMainCharacters()->size());
	if (( charactersCount >= type) && (type >= 0))
		return true;
	return false;
}

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

//TimeManager* GameView::time() {
//	return &_time;
//}