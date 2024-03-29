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
#include "Mission.h"
#include "Daniable.h"

#ifndef event_t
#define event_t
enum event_t {
	EVENT_VIBRATING,
	EVENT_SOUND_ARROW,
	EVENT_SOUND_ATTACK_ON_SHIELD,
	EVENT_SOUND_ATTACK_ON_WOOD,
	EVENT_SOUND_EAT_ITEM,
	EVENT_SOUND_REGENERATION,
	EVENT_SOUND_EXPLOSION,
	EVENT_SOUND_MAGIC,
	EVENT_ICESPELL_ITEMIMPACT,
	EVENT_STOP_ITEMICEUP,
	EVENT_AMMUNITION_CHANGE,
	EVENT_AMMUNITION_DEATH
};
#endif

using namespace std;


class GameView {

public:
	void update();
	~GameView();
	static GameView& instance();
	bool initialize(string chosenMission);
	void restart();
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
	void stopUpdatingPlayer(string userID);
	void setDisconnectedPlayer(string userID);
	/*bool isThereACharInTile(int tileX, int tileY);*/
	Daniable* getDaniableInTile(std::pair <int, int> tile);
	TimeManager* getTimer();
	//void wakeUpPlayer(string userID);
	SpriteAnimado* getErrorImage();
	string manageCharactersPlaying();
	Player* playerWithHighestScore();
	Player* findPlayerByPersonaje(Daniable* personaje);
	int numberOfLoggedInPlayers();
	Mission* getMission();
	void restartPlayers();
	void restartPlayersScores();
	void changeWeapon(string userID, unsigned int weaponIndex);
	bool isThereAPlayerInTile(pair <int,int> pos);
	void addEventUpdate(string _string);
	string manageEventsUpdate();
	string manageUpdate(StringQueue* queue);
	void setPlayersRestarting(bool state);

private:
	GameView();
	Stage worldView;
	vector <Player*> _players;
	TimeManager time;
	CharacterFactory characterFactory;
	SpriteAnimado* errorImage;
	Mission mission;
	StringQueue eventsUpdates;
	bool playersRestarting;

};


#endif