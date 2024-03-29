#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Personaje.h"
#include <string.h>


class Player {

public:
	Player(string userID, Personaje *character);
	~Player();
	//void initialize(float speed, string userID, unsigned int visionRange);
	Personaje *getCharacter();
	void setCharacter(Personaje *character);
	string getUserID();
	void startUpdating();
	void stopUpdating();
	bool isUpdating();
	void missionScore(int score);
	int missionScore();

private:
	string _userID;
	Personaje *_character;
	bool updating;
	int _missionScore;

};

#endif