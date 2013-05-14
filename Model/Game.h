#ifndef _GAME_H_
#define _GAME_H_
#pragma warning(disable: 4512)

#include <fstream>
#include <string>
#include "DataTypes.h"
#include "PersonajeModelo.h"
#include "Configuration.h"
#include "StageModel.h"
////#include "..\View\TimeManager.h"
#include "..\Model\YAMLParser.h"
#include "Player.h"
#include "StringUtilities.h"
////#include "Login.h"
////#include "Connector.h"
////#include "InstructionQueue.h"
////#include "ModelUpdater.h"

class Game
{
public:
	~Game();

	static Game& instance();
	StageModel* world();
	bool initialize();
	bool insidePlayerVision(Player player, std::pair<int,int>);
	bool isKnownByPlayer(Player player, std::pair<int,int>);
	EntLists allEntities;
	YAMLParser yParser;
	AnimatedEntity* animatedEntityAt(unsigned pos);
	EntityObject* entityObjectAt(unsigned pos);
	Configuration* configuration();
	void addPlayer(string userID, string characterName);
	PersonajeModelo* findCharacter(string name);
	string manageMovementUpdate(string userID, string destination);
	void managePositionUpdate(string userID, string position);
	Player* findPlayer(string userID);

private:
	Game();
	StageModel _world;
	Configuration* _configuration;
	vector <Player*> _players;
};

#endif