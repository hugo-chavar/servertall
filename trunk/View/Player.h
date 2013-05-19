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
	string getUserID();
	void startUpdating();
	bool isUpdating();

private:
	string _userID;
	Personaje *_character;
	bool updating;

};

#endif