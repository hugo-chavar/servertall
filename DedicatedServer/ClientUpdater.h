#ifndef _CLIENT_UPDATER_H_
#define _CLIENT_UPDATER_H_

#include "Thread.h"
#include "Client.h"
#include "Listener.h"
#include "InstructionQueue.h"

#define TAMBUFFER 10240

class ClientUpdater : public Thread {
private:
	bool shuttingDown;

	bool available;

	InstructionQueue instructionQueue;

	InstructionQueue& serverInstructionQueue;

	Client* client;

	bool statusOk;

	std::string error;

	bool isShuttingDown();

	void setShuttingDown(bool shuttingDown);

	void setAvailable(bool available);

	InstructionQueue& getInstructionQueue();

	InstructionQueue& getServerInstructionQueue();

	Client* getClient();

	void setStatusOk(bool statusOk);

	void setError(std::string error);

	void updateClient();

	void* run();

	bool sendDirectory(std::string path);

	bool sendFile(std::string path);

	void receiveConfirmation();

	Mutex* mutexUpdates;

public:
	ClientUpdater(InstructionQueue& serverInstructionQueue,Mutex * updaterMutex);

	void setClient(Client* client);

	bool isAvailable();

	bool isStatusOk();

	std::string getError();

	void startClientUpdater();

	void stopClientUpdater();

	~ClientUpdater();
};

#endif // _CLIENT_UPDATER_H_
