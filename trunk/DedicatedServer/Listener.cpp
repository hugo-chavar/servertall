#include "Listener.h"

#include <iostream>


#include "Instruction.h"
#include "StringUtilities.h"

Listener::Listener(unsigned long ip, int port, const int maxPendingConnections, ClientList& preLoggedClients, InstructionQueue& instructionQueue) : preLoggedClients(preLoggedClients), instructionQueue(instructionQueue), socket(ip,port,maxPendingConnections) {
	this->statusOk = true;
	this->error = "";
	this->lastGeneratedUserId = 0;
}

void Listener::startListening() {
	this->start();
}

unsigned int Listener::getLastGeneratedUserId() {
	return this->lastGeneratedUserId;
}

void Listener::setLastGeneratedUserId(unsigned int lastGeneratedUserId) {
	this->lastGeneratedUserId = lastGeneratedUserId;
}

unsigned int Listener::getNextUserId() {
	this->setLastGeneratedUserId(this->getLastGeneratedUserId() + 1);
	return this->getLastGeneratedUserId();
}

Socket Listener::getSocket() {
	return this->socket;
}

ClientList& Listener::getPreLoggedClients() {
	return this->preLoggedClients;
}

InstructionQueue& Listener::getInstructionQueue() {
	return this->instructionQueue;
}

void Listener::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

bool Listener::isStatusOk(){
	return this->statusOk;
}

void Listener::setError(std::string error) {
	this->error = error;
}

std::string Listener::getError() {
	return this->error;
}

void Listener::listen() {
	Socket* newSocket = NULL;
	Client* newClient = NULL;

	if (this->getSocket().bindSocket() != 0) {
		this->setStatusOk(false);
		this->setError("Cannot bind socket to port");
	} else {
		this->getSocket().listenForConnections();

		while (!this->isStopping()) {
			std::cout << "------------------------LISTENING--------------------" << std::endl;
			if ((newSocket = this->getSocket().acceptConnection()) != NULL ) {
				std::cout << "--------------------CONNECTION ACCEPTED--------------------" << std::endl;

				newClient = new Client(newSocket,&(this->getInstructionQueue()),stringUtilities::intToString(this->getNextUserId()),true);
				newClient->startClient();

				this->getPreLoggedClients().addClient(newClient);
			}else{
				if (!this->isStopping()) {
					std::cerr << "Connection error" << std::endl;
				}
			}
		}
	}
}

void* Listener::run() {
	this->listen();
	return NULL;
}

void Listener::stopListening() {
	this->setStopping(true);
	this->getSocket().disconect();
	this->join();
}

Listener::~Listener() {
}
