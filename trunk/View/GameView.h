#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_
#pragma warning(disable: 4512)

#include <fstream>
#include <string>
#include "Personaje.h"
#include "Stage.h"
#include "DataTypes.h"
#include "PersonajeModelo.h"
#include "Configuration.h"
#include "StageModel.h"
#include "..\Model\YAMLParser.h"
#include "Player.h"
#include "StringUtilities.h"
#include "TimeManager.h"


class GameView {

public:
	GameView(void);
	~GameView(void);
	bool initialize(); //
	view::Stage* getWorldView();
	Personaje* getMyPersonaje();
	Personaje* getPersonaje(string name);
	static GameView& instance();

	//DEL MODEL
	StageModel* world();
	//bool initialize();
	bool insidePlayerVision(Player player, std::pair<int,int>);
	bool isKnownByPlayer(Player player, std::pair<int,int>);
	EntLists allEntities;
	YAMLParser yParser;
	AnimatedEntity* animatedEntityAt(unsigned pos);
	EntityObject* entityObjectAt(unsigned pos);
	Configuration* configuration();
	void addPlayer(string userID, string characterType);
	string manageMovementUpdate(string userID, string destination, unsigned int deltaTime);
	void managePositionUpdate(string userID, string position);
	string managePlayersUpdate();
	string managePlayerInitialSynchPosition(string userID);
	string managePlayerInitialSynchVision(string userID);
	Player* findPlayer(string userID);
	bool isCharacterTypeValid(string characterType);
	TimeManager* time();

private:
	view::Stage worldView;
	Personaje* personaje;
	map <string,Personaje*> personajes;//agrego diccionario de personajes
	TimeManager _time;

	//DEL MODEL
	StageModel _world;
	Configuration* _configuration;
	vector <Player*> _players;
};


#endif