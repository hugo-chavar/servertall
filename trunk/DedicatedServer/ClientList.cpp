#include <iostream>

#include "ClientList.h"

// ----------------------------------- CONSTRUCTOR ---------------------------------------

ClientList::ClientList(){
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

Mutex& ClientList::getClientListMutex(){
	return this->clientListMutex;
}

std::list<Client*>& ClientList::getClients(){
	return this->clients;
}

//IMPORTANT: CALL ONLY AFTER LOCK.
std::list<Client*>::iterator ClientList::findClient(std::string userID){
	std::list<Client*>::iterator i = this->getClients().begin();
	bool found = false;

	while( (i != this->getClients().end()) && (!found) ){
		if ((*i)->getUserID() == userID){
			found = true;
		}else{
			++i;
		}
	}

	return i;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

bool ClientList::isUserIDAvailable(std::string userID) {
	bool available = false;

	if (this->getClient(userID) != NULL) // CAMBIADO DE == A !=
		available = true;

	return available;
}

void ClientList::addClient(Client* client){
	this->getClientListMutex().lock();

	this->getClients().push_back(client);

	this->getClientListMutex().unlock();
}

Client* ClientList::getClient(std::string userID) {
	Client* client = NULL;

	this->getClientListMutex().lock();

	std::list<Client*>::iterator i = this->findClient(userID);
	if (i != this->getClients().end())
		client = *i;

	this->getClientListMutex().unlock();

	return client;
}

Client* ClientList::detachClient(std::string userID) {
	Client* client = NULL;

	this->getClientListMutex().lock();

	std::list<Client*>::iterator it = this->findClient(userID);
	if (it != this->getClients().end()) {
		client = *it;
		*it = NULL;
		this->getClients().erase(it);
	} else {
		std::cout << "YOU SHOULD'N BE HERE KID >=(" << std::endl;
	}

	this->getClientListMutex().unlock();

	return client;
}

bool ClientList::addInstructionTo(Instruction instruction, std::string userID) {
	Client* client = this->getClient(userID);

	if (client != NULL) {
		client->addInstruction(instruction);
		return true;
	} else {
		return false;
	}
}

void ClientList::addBroadcastFrom(Instruction instruction, std::string userID) {
	this->getClientListMutex().lock();

	for (std::list<Client*>::iterator it = this->getClients().begin(); it != this->getClients().end(); ++it) {
		if (userID != (*it)->getUserID())
			(*it)->addInstruction(instruction);
	}

	this->getClientListMutex().unlock();
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

ClientList::~ClientList(){
	for(std::list<Client*>::iterator it = this->getClients().begin(); it != this->getClients().end(); ++it){
		//ONLY CASE WHERE STOP IS UP TO THE CLIENT LIST. IDEALLY AT THIS POINT EVERY CLIENT MUST HAVE BEEN
		//DETACHED AND STOPED BY THE CLASS CONTAINING THE LIST.
		(*it)->stopClient();
		delete *it;
	}
}
