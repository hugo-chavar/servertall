#include "Game.h"
#include "Constants.h"


Game::Game() {
}

Game::~Game() {
//	_login.cleanUp();
}

Game& Game::instance() {
	static Game singleton;
	return singleton;
}

StageModel* Game::world() {
	return &_world;
}

bool Game::initialize() {
	yParser.parse();
	_world = yParser.vStages()[0];
	unsigned stageActual = 0;
	allEntities = yParser.allLists();
	_configuration = yParser.getConfig();

	//si hubieron problemas salgo
	if (!_configuration)
		return false;

	return true;
}

EntityObject* Game::entityObjectAt(unsigned pos) {
	if (allEntities.vEntitiesObject.size() > pos)
		return allEntities.vEntitiesObject.at(pos);
	Logger::instance().nullPointer("function EntityObject* Game::animatedEntityAt");
	return NULL;
}


AnimatedEntity* Game::animatedEntityAt(unsigned pos) {
	if (allEntities.vAnimatedEntities.size() > pos)
		return allEntities.vAnimatedEntities.at(pos);
	Logger::instance().nullPointer("function AnimatedEntity* Game::animatedEntityAt");
	return NULL;
}

PersonajeModelo* Game::findCharacter(string name) {
	bool found = false;
	PersonajeModelo* character = NULL;
	int i = 0;
	while ((i<_world.vMainCharacters()->size()) && (!found)) {
		if (_world.vMainCharacters()->at(i)->getAnimation()->name()==name) {
			found = true;
			character = _world.vMainCharacters()->at(i);
		}
		else
			i++;
	}
	return character;
}

void Game::addPlayer(string userID, string characterName) {
	PersonajeModelo *character = findCharacter(characterName);
	Player *player = new Player(userID, character);
	player->getCharacter()->setVelocidad(_configuration->mainCharacterSpeed());
	////player.getCharacter()->setName(nombreJugador);
	player->getCharacter()->createVision(_configuration->visionRange());
	_players.push_back(player);
}

string Game::manageMovementUpdate(string userID, string destination) {
	Player *player = findPlayer(userID);
	pair <int, int> current = player->getCharacter()->getPosition();
	pair <int, int> pair_destination = stringUtilities::stringToPairInt(destination);
	player->getCharacter()->setDestino(pair_destination.first, pair_destination.second);
	bool finished = false;
	int numberOfTiles = 0;
	string movementArgument = "";
	string str_nextTiles = "";
	while (!finished) {
		pair <int, int> nextTile = player->getCharacter()->mover(pair_destination);
		if (nextTile.first<0) {
			numberOfTiles = 0;
			movementArgument = stringUtilities::intToString(numberOfTiles);
			return movementArgument;
		}
		str_nextTiles = str_nextTiles+","+stringUtilities::pairIntToString(nextTile);
		player->getCharacter()->setCurrent(nextTile.first, nextTile.second);
		if ((nextTile.first==pair_destination.first) && (nextTile.second==pair_destination.second))
			finished = true;
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

void Game::managePositionUpdate(string userID, string position) {
	pair <int, int> pair_position = stringUtilities::stringToPairInt(position);
	Game::instance().findPlayer(userID)->getCharacter()->setCurrent(pair_position.first, pair_position.second);
}

Player* Game::findPlayer(string userID) {
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

Configuration* Game::configuration() {
	if (_configuration)
			return _configuration;
	Logger::instance().nullPointer("Configuration* Game::configuration");
	return NULL;
}

bool Game::insidePlayerVision(Player player, std::pair<int,int> pos){
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

bool Game::isKnownByPlayer(Player player, std::pair<int,int> pos) {
	return player.getCharacter()->getVision()->testPosition(pos);
}