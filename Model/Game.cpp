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
	std::string command;
	std::vector<std::string> scenarios;
	unsigned int scenario = 0;
	bool scenarioPicked = false;

	for(unsigned i=0; i< yParser.vStages().size(); i++)
		scenarios.push_back(yParser.vStages()[i].name());

	while (!scenarioPicked){
		std::cout << "Please choose a scenario: " << std::endl;

		for (unsigned int i = 0; i < scenarios.size(); i++) {
			std::cout << stringUtilities::unsignedToString(i + 1) << " - " << scenarios[i] << std::endl;
		}

		std::cin >> command;

		scenario = stringUtilities::stringToUnsigned(command); 
		if (scenario > 0 && scenario <= scenarios.size()) {
			scenarioPicked = true;
			std::cout << "Loading escenario: " << scenarios[scenario - 1] << std::endl;
		} else {
			std::cout << "There is no escenario with ID " << stringUtilities::unsignedToString(scenario) << std::endl;			
		}
	}

	Game::instance().setStageActual(scenario - 1);

	_world = yParser.vStages()[this->stageActual()];

	allEntities = yParser.allLists();
	_world.loadNamedChars();
	_configuration = yParser.getConfig();
	//this->_time.initializeTime();
	Logger::instance().setFile(LOGFILE);
	Logger::instance().log("Iniciando el juego..");

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

void Game::setStageActual(unsigned stageActual) {
	this->_stageActual = stageActual;
}