#include "ClientUpdater.h"

#include <iostream>

#include "Instruction.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

ClientUpdater::ClientUpdater() {
	this->shuttingDown = false;
	this->statusOk = true;
	this->error = "";
	this->available = true;
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

bool ClientUpdater::isShuttingDown() {
	return this->shuttingDown;
}

void ClientUpdater::setShuttingDown(bool shuttingDown) {
	this->shuttingDown = shuttingDown;
}

void ClientUpdater::setAvailable(bool available) {
	this->available = available;
}

Client* ClientUpdater::getClient() {
	return this->client;
}

void ClientUpdater::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void ClientUpdater::setError(std::string error) {
	this->error = error;
}

void ClientUpdater::updateClient() {
	Instruction instructionIn;
	Instruction instructionOut;
	Client* client = NULL;
	std::string argument = "";
	bool finished = false;

	while(!this->isShuttingDown() && !finished){
		instructionOut.clear();

		//ACA SE DEBE DEFINIR COMO SE VA A MANEJAR LA LISTA DE ARCHIVOS A ENVIAR Y SI REQUIERE CONFIRMACION O NO DEL CLIENTE.
		//POR AHORA MANDA UN MENSAJE DUMMY Y TERMINA.

		instructionOut.setOpCode(OPCODE_UPDATE_FILE);
		instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_FILE,"THIS SHOULD BE AN ACTUAL SERIALIZED FILE");
		this->getClient()->addInstruction(instructionOut);

		finished = true;
	}
	
	instructionOut.clear();
	instructionOut.setOpCode(OPCODE_UPDATE_COMPLETE);
	this->getClient()->addInstruction(instructionOut);

	this->stopClientUpdater();
}


void* ClientUpdater::run() {
	this->updateClient();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

bool ClientUpdater::isAvailable() {
	return this->available;
}

bool ClientUpdater::isStatusOk() {
	return this->statusOk;
}

std::string ClientUpdater::getError() {
	return this->error;
}

void ClientUpdater::setClient(Client* client) {
	this->client = client;
}

void ClientUpdater::startClientUpdater() {
	this->start();
}

void ClientUpdater::stopClientUpdater() {
	this->setClient(NULL);
	this->setShuttingDown(true);
	this->join();
	this->setAvailable(true);
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

ClientUpdater::~ClientUpdater() {
}
