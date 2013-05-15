#include <iostream>

#include "Client.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

Client::Client(Socket* socket, InstructionQueue* instructionQueue, std::string userID, bool inyectUserIDonReceive) : connector(socket,instructionQueue,userID,inyectUserIDonReceive) {
	this->active = false;
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

// NO PRIVATE METHODS.

// ----------------------------------- PUBLIC METHODS ------------------------------------

bool Client::isActive() {
	return this->active;
}

void Client::setActive(bool active) {
	this->active = active;
}

Connector& Client::getConnector() {
	return this->connector;
}

std::string Client::getUserID() {
	return this->getConnector().getUserID();
}

void Client::setUserID(std::string userID) {
	this->getConnector().setUserID(userID);
}

Socket* Client::getSocket() {
	return this->getConnector().getSocket();
}

void Client::addInstruction(Instruction& instruction) {
	this->getConnector().addInstruction(instruction);
}

void Client::addBroadcast(Instruction& instruction) {
	this->getConnector().addBroadcast(instruction);
}

void Client::startClient() {
	this->getConnector().startConnector();
}

void Client::stopClient() {
	this->getConnector().stopConnector(true);
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

Client::~Client(){
}
