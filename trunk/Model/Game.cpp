#include "Game.h"
#include "Constants.h"
#include <time.h>


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
	//initialize random number generator
	srand((unsigned)time(NULL));

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
	//_world.generateItems(5);//HARCODEO PORCENTAJE ITEMS
	_configuration = yParser.getConfig();
	//this->_time.initializeTime();
	Logger::instance().setFile(LOGFILE);
	Logger::instance().log("Iniciando el juego..");

	return true;
}

void Game::restart() {
	//initialize random number generator
	srand((unsigned)time(NULL));

	unsigned int scenario = this->stageActual() + 1;
	if (scenario < yParser.vStages().size())
		Game::instance().setStageActual(scenario);
	else
		Game::instance().setStageActual(0);

	_world = yParser.vStages()[this->stageActual()];
	//_world.generateItems(5);//HARCODEO PORCENTAJE ITEMS
	//this->_time.initializeTime();
}

float Game::getRandom() {
	float max = 1.0;
	float min = 0.0;
	return (float)(((float) rand() / (((float)RAND_MAX) + 1.0)) * (max-min+1) + min);
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