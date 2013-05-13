#include "Player.h"

Player::Player(string userID, PersonajeModelo *character) {
	_userID = userID;
	_character = character;
}

Player::~Player() { }

PersonajeModelo *Player::getCharacter() {
	return _character;
}