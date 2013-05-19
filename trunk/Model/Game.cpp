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
	yParser.parse(CONFIGFILE_DIRECTORY,false);
	_world = yParser.vStages()[0];
	_stageActual = 0;
	allEntities = yParser.allLists();
	_world.loadNamedChars();
	//_configuration = yParser.getConfig();
	//this->_time.initializeTime();

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
	return &_configuration;
}

unsigned Game::stageActual() {
	return _stageActual;
}