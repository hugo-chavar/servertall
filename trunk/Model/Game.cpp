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

void Game::addPlayer(Player player) {
	player.getCharacter()->setVelocidad(_configuration->mainCharacterSpeed());
	////player.getCharacter()->setName(nombreJugador);
	player.getCharacter()->createVision(_configuration->visionRange());
	_players.push_back(player);
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