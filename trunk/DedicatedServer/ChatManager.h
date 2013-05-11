#ifndef _CHAT_MANAGER_H_
#define _CHAT_MANAGER_H_

#include "Thread.h"
#include "ClientList.h"
#include "InstructionQueue.h"

class ChatManager : public Thread {
private:
	ClientList clients;

	InstructionQueue instructionQueue;

	bool statusOk;

	std::string error;

	void setStatusOk(bool statusOk);

	void setError(std::string error);

	void processRequests();

	void* run();

public:
	ChatManager();

	ClientList& getClients();

	InstructionQueue& getInstructionQueue();

	bool isStatusOk();

	std::string getError();

	void startChatManager();

	void stopChatManager();

	~ChatManager();
};

#endif // _CHAT_MANAGER_H_
