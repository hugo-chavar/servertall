#ifndef _SIMULATION_MANAGER_H_
#define _SIMULATION_MANAGER_H_

#include "Thread.h"
#include "ClientList.h"
#include "InstructionQueue.h"

#ifndef DESIREDFPS
#define DESIREDFPS 50
#endif//DESIREDFPS

class SimulationManager : public Thread {
private:
	ClientList clients;

	InstructionQueue instructionQueue;

	bool statusOk;

	int numberOfRestartedClients;

	std::string error;

	void setStatusOk(bool statusOk);

	void setError(std::string error);

	void simulate();

	void processInstruction(Instruction instruction);

	Instruction manageInitSynchronize(std::string userID);
	
	Instruction manageGeneralInitSynchronize(std::string userID);

	void* run();

	std::string lastBroadcast;

public:
	SimulationManager();

	ClientList& getClients();

	InstructionQueue& getInstructionQueue();

	bool isStatusOk();

	std::string getError();

	void startSimulationManager();

	void stopSimulationManager();

	~SimulationManager();
};

#endif // _SIMULATION_MANAGER_H_
