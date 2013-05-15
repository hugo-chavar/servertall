#include "Player.h"

Player::Player(string userID, Personaje *character) {
	_userID = userID;
	_character = character;
}

Player::~Player() { }

Personaje *Player::getCharacter() {
	return _character;
}

string Player::getUserID() {
	return _userID;
}