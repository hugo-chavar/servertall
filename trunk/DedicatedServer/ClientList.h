#ifndef _CLIENTLIST_H_
#define _CLIENTLIST_H_

#include <list>

#include "Mutex.h"
#include "ConditionVariable.h"
#include "Client.h"
#include "Instruction.h"

class ClientList{
private:
	Mutex clientListMutex;

	std::list<Client*> clients;

	ConditionVariable broadcastConditionVariable;

	Mutex& getClientListMutex();

	std::list<Client*>& getClients();

	ConditionVariable& getBroadcastConditionVariable();

	std::list<Client*>::iterator findClient(std::string userID);

public:
	ClientList();

	bool isUserIDAvailable(std::string userID);

	void addClient(Client* client);

	Client* getClient(std::string userID);

	Client* detachClient(std::string userID);

	bool addInstructionTo(Instruction& instruction, std::string to);

	void addBroadcast(Instruction& instruction, std::string from = "");

	//TODO: ADD BROADCAST MECHANISM TO SENDERS.
	//void broadcast();

	~ClientList();
};

#endif // _CLIENTLIST_H_