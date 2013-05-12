#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_

#ifndef MAX_FILE_UPDATERS
#define MAX_FILE_UPDATERS 3
#endif//MAX_FILE_UPDATERS

#include "Thread.h"
#include "ClientList.h"
#include "ClientUpdater.h"
#include "Listener.h"
#include "InstructionQueue.h"
#include "ChatManager.h"
#include "SimulationManager.h"

class LoginManager : public Thread {
private:
	ClientList preLoggedClients;

	ClientList loggedClients;

	InstructionQueue instructionQueue;

	Listener listener;

	//This probably should be in the config file.
	unsigned int maxFileUpdaters;

	std::vector<ClientUpdater> clientUpdaters;

	ChatManager& chatManager;

	SimulationManager& simulationManager;

	bool statusOk;

	std::string error;

	ClientList& getPreLoggedClients();

	ClientList& getLoggedClients();

	InstructionQueue& getInstructionQueue();

	Listener& getListener();

	unsigned int getMaxFileUpdaters();

	std::vector<ClientUpdater>& getClientUpdaters();

	ChatManager& getChatManager();

	SimulationManager& getSimulationManager();

	void setStatusOk(bool statusOk);

	void setError(std::string error);

	void startListener();

	void stopListener();

	void processRequests();

	void createClientUpdater();

	void* run();

public:
	LoginManager(int portToListen, int maxPendingConnections, ChatManager& chatManager, SimulationManager& simulationManager);

	bool isStatusOk();

	std::string getError();

	void startLoginManager();

	void stopLoginManager();

	~LoginManager();
};

#endif // _LOGIN_MANAGER_H_
