#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "PersonajeModelo.h"


class Player {

public:
	Player(int userID, PersonajeModelo *character);
	~Player();
	PersonajeModelo *getCharacter();

private:
	int _userID;
	PersonajeModelo *_character;

};

#endif