#pragma warning(disable: 4146)
#ifndef __YAMLPARSER_H__
#define __YAMLPARSER_H__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "..\Libraries\Parser YAML\include\yaml-cpp\yaml.h"
#include "Logger.h"
#include "DataTypes.h"
#include "StageModel.h"
#include "DirList.h"
#include "PersonajeModelo.h"
#include "Configuration.h"

using namespace std;
using namespace YAML;
using namespace common;


// Data types

typedef enum ForceDefault
{
	YES_IGNORE_LOG,
	ONLY_INVALID,
	YES,
	NO
};

struct Screen {
	int width;
	int height;
};

struct Config {
	float main_character_speed;
	int scroll_margin;
	//int port;
	int vision_range;
};

struct Connection {
	int port;
	std::string ip;
};

struct Entities {
	vector <EntityObject*> vEntitiesObject;
	vector <AnimatedEntity*> vAnimatedEntities;
};

struct sMainCharacter {
	string entityType;
	int x;
	int y;
	float minPrecision;
	float maxDamage;
	float maxLife;
	float maxMagic;
};

struct sItemsDef {
	float percentage;
	vector <sItem> vItems;
};

struct sStage {
	string name;
	int size_x;
	int size_y;
	vector <EntityDef> vEntitiesDef;
	sItemsDef itemsDef;
	vector <sMainCharacter> vMainCharacters_aux;
	vector <PersonajeModelo*> vMainCharacters;
	map <string,PersonajeModelo*> mapMainCharacters;
};

struct Stages {
	vector <sStage> vStages_aux;
	vector <StageModel> vStages;
};


// Class

class YAMLParser {

private:
	Screen screen;
	Configuration game_config;
	Config configuration;
	Connection connection;
	EntLists entities;
	Stages stages;

	Screen generateDefaultScreen();
	Config generateDefaultConfiguration();
	PersonajeModelo* generateDefaultMainCharacter();
	StageModel generateDefaultStage();
	void loadEverythingByDefault();
	void loadEntitiesToMap(int stage_index);
	void loadMainCharacters(int stage_index);
	void loadItemsToMap(int stage_index);
	void manageEntityCase();
	void manageStageCase();
	bool entityBaseIsInMapRange(int entityDef_index, sStage stage_aux, EntityObject* entityObjectType, AnimatedEntity* animatedEntityType);

public:
	YAMLParser();
	~YAMLParser();

	void parse(string directory, bool connecting);
	vector <StageModel> vStages();
	EntLists allLists();

	AnimatedEntity* findAnimatedEntityType(string name);
	EntityObject* findEntityObjectType(string name);
	/*PersonajeModelo* modelMainCharacters(unsigned, unsigned);*/
	Configuration getConfig();
	int getConfigPort();
	std::string getConfigIp();
};


#endif