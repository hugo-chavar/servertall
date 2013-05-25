#include "Listener.h"

#include "Instruction.h"
#include "StringUtilities.h"
#include "CraPPyLog.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

Listener::Listener(unsigned long ip, int port, const int maxPendingConnections, ClientList& preLoggedClients, InstructionQueue& instructionQueue) : preLoggedClients(preLoggedClients), instructionQueue(instructionQueue), socket(ip,port,maxPendingConnections) {
	this->statusOk = true;
	this->error = "";
	this->lastGeneratedUserId = 0;
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

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

ClientList& Listener::getPreLoggedClients() {
	return this->preLoggedClients;
}

InstructionQueue& Listener::getInstructionQueue() {
	return this->instructionQueue;
}

Socket Listener::getSocket() {
	return this->socket;
}

void Listener::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void Listener::setError(std::string error) {
	this->error = error;
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
			if ((newSocket = this->getSocket().acceptConnection()) != NULL ) {
				LOG_DEBUG("CONNECTION ACCEPTED");

				newClient = new Client(newSocket,&(this->getInstructionQueue()),stringUtilities::intToString(this->getNextUserId()),true);
				newClient->startClient();

				this->getPreLoggedClients().addClient(newClient);
			}else{
				if (!this->isStopping()) {
					LOG_ERROR("CONNECTION ERROR");
				}
			}
		}
	}
}

void* Listener::run() {
	this->listen();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

void Listener::startListening() {
	this->start();
	LOG_DEBUG("LISTENER THREAD STARTED");
}

bool Listener::isStatusOk(){
	return this->statusOk;
}

std::string Listener::getError() {
	return this->error;
}

void Listener::stopListening() {
	this->setStopping(true);
	this->getSocket().disconect();
	this->join();
	LOG_DEBUG("LISTENER THREAD STOPPED");
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

Listener::~Listener() {
}