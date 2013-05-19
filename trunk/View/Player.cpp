#include "Player.h"

Player::Player(string userID, Personaje *character) {
	_userID = userID;
	_character = character;
	this->updating = false;
}

Player::~Player() { }

//void Player::initialize(float speed, string userID, unsigned int visionRange) {
//	_character->personajeModelo()->setVelocidad(speed);
//	_character->personajeModelo()->setName(userID);
//	_character->personajeModelo()->createVision(visionRange);
//	_character->loadSprites();
//}

Personaje *Player::getCharacter() {
	return _character;
}

string Player::getUserID() {
	return _userID;
}

void Player::startUpdating() {
	this->updating = true;
}

bool Player::isUpdating() {
	return this->updating;
}