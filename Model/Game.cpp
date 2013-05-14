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
	int numberOfTiles = 1;
	string movementArgument = stringUtilities::intToString(numberOfTiles);
	pair <int, int> nextTile = player->getCharacter()->mover(stringUtilities::stringToPairInt(destination));
	if (nextTile.first<0) {
		numberOfTiles = 0;
		movementArgument = stringUtilities::intToString(numberOfTiles);
		return movementArgument;
	}
	player->getCharacter()->setCurrent(nextTile.first, nextTile.second);
	string str_nextTile = stringUtilities::pairIntToString(nextTile);
	movementArgument = movementArgument+","+str_nextTile;
	return movementArgument;
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