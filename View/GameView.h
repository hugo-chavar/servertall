#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_
//#pragma warning(disable: 4512)

#include <fstream>
#include <string>
#include "Stage.h"
#include "Game.h"
#include "Player.h"
#include "CharacterFactory.h"
#include "TimeManager.h"

using namespace std;


class GameView {

public:
	void update();
	~GameView();
	static GameView& instance();
	bool initialize();
	Stage* getWorldView();
	//Personaje* getPersonaje(string name);
	void addPlayer(string userID, string characterType);
	Player* findPlayer(string userID);
	bool isCharacterTypeValid(string characterType);
	bool insidePlayerVision(Player player, std::pair<int,int>);
	bool isKnownByPlayer(Player player, std::pair<int,int>);
	void manageMovementUpdate(string userID, string destination); //, unsigned int deltaTime
	void manageAnimationUpdate(string userID, string animation);
	string managePlayersUpdate();
	string managePlayerInitialSynch(string userID);
	void startUpdatingPlayer(string userID);
	void setDisconnectedPlayer(string userID);
	bool isThereACharInTile(int tileX, int tileY);
	Personaje* getCharInTile(std::pair <int, int> tile);
	TimeManager* getTimer();
	//void wakeUpPlayer(string userID);
	SpriteAnimado* getErrorImage();
	string manageCharactersPlaying();
private:
	GameView();
	Stage worldView;
	vector <Player*> _players;
	TimeManager time;
	CharacterFactory characterFactory;
	SpriteAnimado* errorImage;

};


#endif