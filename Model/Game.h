#ifndef _GAME_H_
#define _GAME_H_
#pragma warning(disable: 4512)


#include <fstream>
#include <string>
#include "DataTypes.h"
#include "PersonajeModelo.h"
#include "Configuration.h"
#include "StageModel.h"
//#include "TimeManager.h"
#include "YAMLParser.h"
#include "StringUtilities.h"


class Game
{
public:
	~Game();
	static Game& instance();
	StageModel* world();
	unsigned stageActual();
	void setStageActual(unsigned stageActual);
	bool initialize();
	void restart();
	//TimeManager* time();
	EntLists allEntities;
	YAMLParser yParser;
	AnimatedEntity* animatedEntityAt(unsigned pos);
	EntityObject* entityObjectAt(unsigned pos);
	Configuration* configuration();
	float getRandom();

private:
	Game();
	StageModel _world;
	unsigned _stageActual;
	//TimeManager _time;
	Configuration _configuration;
	int stageNumber;
};

#endif