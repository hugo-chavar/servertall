#ifndef _SIMULATION_MANAGER_H_
#define _SIMULATION_MANAGER_H_

#include "Thread.h"
#include "ClientList.h"
#include "InstructionQueue.h"


class SimulationManager : public Thread {
private:
	ClientList clients;

	InstructionQueue instructionQueue;

	bool statusOk;

	std::string error;

	void setStatusOk(bool statusOk);

	void setError(std::string error);

	void simulate();

	void processInstruction(Instruction instruction);

	void* run();

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
