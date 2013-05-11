#ifndef _CLIENTLIST_H_
#define _CLIENTLIST_H_

#include <list>

#include "Mutex.h"
#include "Client.h"
#include "Instruction.h"

class ClientList{
private:
	Mutex clientListMutex;

	std::list<Client*> clients;

	Mutex& getClientListMutex();

	std::list<Client*>& getClients();

	std::list<Client*>::iterator findClient(std::string userID);

public:
	ClientList();

	bool isUserIDAvailable(std::string userID);

	void addClient(Client* client);

	Client* getClient(std::string userID);

	Client* detachClient(std::string userID);

	bool addInstructionTo(Instruction instruction, std::string userID);

	void addBroadcastFrom(Instruction instruction, std::string userID);

	//TODO: ADD BROADCAST MECHANISM TO SENDERS.
	//void broadcast();

	~ClientList();
};

#endif // _CLIENTLIST_H_