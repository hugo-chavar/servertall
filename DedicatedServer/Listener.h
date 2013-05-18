#ifndef _LISTENER_H_
#define _LISTENER_H_

#pragma warning(disable: 4512)

#include "Thread.h"
#include "Socket.h"
#include "ClientList.h"

class Listener : public Thread{
private:
	unsigned int lastGeneratedUserId;

	ClientList& preLoggedClients;

	InstructionQueue& instructionQueue;

	Socket socket;

	bool statusOk;

	std::string error;

	unsigned int getLastGeneratedUserId();

	void setLastGeneratedUserId(unsigned int lastGeneratedUserId);

	unsigned int getNextUserId();

	ClientList& getPreLoggedClients();

	InstructionQueue& getInstructionQueue();

	Socket getSocket();

	void setStatusOk(bool statusOk);

	void setError(std::string error);

	void listen();

	void* run();

public:
	//TODO: WHY IS PORT AN INT?!?!?
	Listener(unsigned long ip, int port, const int maxPendingConnections, ClientList& preLoggedClients, InstructionQueue& instructionQueue);

	void startListening();

	bool isStatusOk();

	std::string getError();

	void stopListening();

	~Listener();
};

#endif // _LISTENER_H_
