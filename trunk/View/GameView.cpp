#include "GameView.h"


GameView::GameView(void) { }

GameView::~GameView(void) {
//	_login.cleanUp();
}

bool GameView::initialize() //view::Camera camera
{
	// DEL MODEL
	yParser.parse();
	_world = yParser.vStages()[0];
	unsigned stageActual = 0;
	allEntities = yParser.allLists();
	_configuration = yParser.getConfig();

	//si hubieron problemas salgo
	if (!_configuration)
		return false;

	return true;

	//Yami este metodo todavia no esta.Hay que ver que cosas de aca van, porq es una copia del del modelo por ahora

	//	bool mapInitialized = worldView.initialize();
	//	bool chatInitialized = chat.initialize(camera);
	//	return (mapInitialized && chatInitialized);

	//YAMLParser connectionParser;
	//connectionParser.parse(CONNECTION_DIRECTORY, true);
	//int serverPortNumber = connectionParser.getConfigPort();
	//std::string serverIpAddress = connectionParser.getConfigIp();

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
	//return true;

}

GameView& GameView::instance() {
	static GameView singleton;
	return singleton;
}

view::Stage* GameView::getWorldView() {
	return &worldView;
}

Personaje* GameView::getPersonaje(string name) {
	Personaje* personaje = NULL;
	std::map<std::string,Personaje*>::iterator it = this->personajes.find(name);

	if (it != this->personajes.end())
		personaje = it->second;

	return personaje;
}

Personaje* GameView::getMyPersonaje() {
	if (this->personaje){
		return this->personaje;
	}
	Logger::instance().nullPointer("function Personaje* GameView::getMyPersonaje");
	return NULL;
}

Configuration* GameView::configuration() {
	return &_configuration;
}

void GameView::setPlayerName(string name) {
	this->playerName=name;
}

string GameView::getPlayerName() {
	return this->playerName;
}

void GameView::setPlayerCharacterId(string char_id) {
	playerCharacterId=char_id;
}

string GameView::getPlayerCharacterId() {
	return this->playerCharacterId;
}


// DEL MODEL

StageModel* GameView::world() {
	return &_world;
}

EntityObject* GameView::entityObjectAt(unsigned pos) {
	if (allEntities.vEntitiesObject.size() > pos)
		return allEntities.vEntitiesObject.at(pos);
	Logger::instance().nullPointer("function EntityObject* Game::animatedEntityAt");
	return NULL;
}

AnimatedEntity* GameView::animatedEntityAt(unsigned pos) {
	if (allEntities.vAnimatedEntities.size() > pos)
		return allEntities.vAnimatedEntities.at(pos);
	Logger::instance().nullPointer("function AnimatedEntity* Game::animatedEntityAt");
	return NULL;
}

bool GameView::isCharacterTypeValid(string characterType) {
	int type = stringUtilities::stringToInt(characterType);
	if (_world.vMainCharacters()->size()>type)
		return true;
	return false;
}

void GameView::addPlayer(string userID, string characterType) {
	PersonajeModelo* character = NULL;
	PersonajeModelo aux = *_world.vMainCharacters()->at(stringUtilities::stringToInt(characterType));
	character = &aux;
	Player *player = new Player(userID, character);
	player->getCharacter()->setVelocidad(_configuration->mainCharacterSpeed());
	player->getCharacter()->setName(userID);
	player->getCharacter()->createVision(_configuration->visionRange());
	_players.push_back(player);
}

string GameView::manageMovementUpdate(string userID, string destination, unsigned int deltaTime) {
	Player *player = findPlayer(userID);
	pair <int, int> current = player->getCharacter()->getPosition();
	pair <int, int> pair_destination = stringUtilities::stringToPairInt(destination);
	player->getCharacter()->setDestino(pair_destination.first, pair_destination.second);
	bool finished = false;
	int numberOfTiles = 0;
	string movementArgument = "";
	string str_nextTiles = "";
	while (!finished) {
		player->getCharacter()->mover(deltaTime);
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
	player->getCharacter()->setCurrent(current.first, current.second);
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

void GameView::managePositionUpdate(string userID, string position) {
	pair <int, int> pair_position = stringUtilities::stringToPairInt(position);
	Game::instance().findPlayer(userID)->getCharacter()->setCurrent(pair_position.first, pair_position.second);
}

string GameView::managePlayersUpdate() {
	string argument = "";
	for (int i=0; i<_players.size(); i++)
		argument = _players[i]->getUserID()+","+_players[i]->getCharacter()->toString()+":";
	argument.pop_back();
	return argument;
}

string GameView::managePlayerInitialSynchPosition(string userID) {
	Player *player = findPlayer(userID);
	string position = stringUtilities::pairIntToString(player->getCharacter()->getPosition());
	return position;
}

string GameView::managePlayerInitialSynchVision(string userID) {
	Player *player = findPlayer(userID);
	string vision = player->getCharacter()->getVision()->toString();
	return vision;
}

Player* GameView::findPlayer(string userID) {
	bool found = false;
	Player* player = NULL;
	int i = 0;
	while ((i<_players.size()) && (!found)) {
		if (_players[i]->getUserID()==userID) {
			found = true;
			player = _players[i];
		}
		else
			i++;
	}
	return player;
}

Configuration* GameView::configuration() {
	if (_configuration)
			return _configuration;
	Logger::instance().nullPointer("Configuration* Game::configuration");
	return NULL;
}

bool GameView::insidePlayerVision(Player player, std::pair<int,int> pos){
	bool inside = player.getCharacter()->getVision()->isInsideVision(pos);

	if (!inside) {
		TileModel* relatedTile = Game::instance().world()->getTileAt(pos)->getRelatedTile();
		if (relatedTile) {//TODO: mejorar para optimizar codigo
			// preguntar si es drawable() e ir salteando..
			while ( (!inside) && (relatedTile != Game::instance().world()->getTileAt(pos)) ) {
				pair<int, int> posRelated = relatedTile->getPosition();
				inside = player.getCharacter()->getVision()->isInsideVision(posRelated);
				relatedTile = relatedTile->getRelatedTile();
			}
		}
	}
	return inside;
}

bool GameView::isKnownByPlayer(Player player, std::pair<int,int> pos) {
	return player.getCharacter()->getVision()->testPosition(pos);
}