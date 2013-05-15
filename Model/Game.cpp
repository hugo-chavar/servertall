#include "Game.h"
#include "Constants.h"


Game::Game() { }

Game::~Game() { }

Game& Game::instance() {
	static Game singleton;
	return singleton;
}

StageModel* Game::world() {
	return &_world;
}

//TimeManager* Game::time() {
//	return &_time;
//}

bool Game::initialize() {
	yParser.parse();
	_world = yParser.vStages()[0];
	unsigned stageActual = 0;
	allEntities = yParser.allLists();
	_configuration = yParser.getConfig();
	//this->_time.initializeTime();
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

Configuration* Game::configuration() {
	if (_configuration)
			return _configuration;
	Logger::instance().nullPointer("Configuration* Game::configuration");
	return NULL;
}