#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "PersonajeModelo.h"


class Player {

public:
	Player(string userID, PersonajeModelo *character);
	~Player();
	PersonajeModelo *getCharacter();

private:
	string _userID;
	PersonajeModelo *_character;

};

#endif