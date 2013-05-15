#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Personaje.h"
#include <string.h>


class Player {

public:
	Player(string userID, Personaje *character);
	~Player();
	Personaje *getCharacter();
	string getUserID();

private:
	string _userID;
	Personaje *_character;

};

#endif