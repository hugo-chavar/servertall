#include "SimulationManager.h"

#include "SDL.h"
#include "StringUtilities.h"

#include <iostream>


// ----------------------------------- CONSTRUCTOR ---------------------------------------

SimulationManager::SimulationManager() {
	this->statusOk = true;
	this->error = "";
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

void SimulationManager::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void SimulationManager::setError(std::string error) {
	this->error = error;
}

void SimulationManager::simulate() {
	Instruction instructionIn;
	Instruction instructionOut;
	//TODO: create FPS manager.
	float milisecondsTonextFrame = static_cast<float>(1000)/60; //HARDCODED FPS
	unsigned int frameStartedAt = 0;
	
	unsigned int i = 0;

	while(!this->isStopping()) {
		frameStartedAt = SDL_GetTicks();

		this->getInstructionQueue().lock();
		while (this->getInstructionQueue().getInstructionQueue().size() != 0) {
			instructionIn = this->getInstructionQueue().getInstructionQueue().front();
			processInstruction(instructionIn);
			this->getInstructionQueue().getInstructionQueue().pop();
		}
		this->getInstructionQueue().unLock();

		// AVANZAR LA SIMULACIÓN UN DELTA DE TIEMPO.

		// HACER UN BROADCAST DEL UPDATE A LOS CLIENTES
		instructionOut.clear();
		instructionOut.setOpCode(OPCODE_SIMULATION_UPDATE);
		//std::string argument = GameView::instance().managePlayersUpdate();
		std::string argument = ""; //reemplazar x linea de arriba cuando compile GameView
		instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SIMULATION_UPDATE, argument);
		this->getClients().addBroadcast(instructionOut);

		//instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SIMULATION_UPDATE,"DUMMY UPDATE" + stringUtilities::unsignedToString(i));
		//this->getClients().addBroadcast(instructionOut);
		i++;

		if (milisecondsTonextFrame >= SDL_GetTicks() - frameStartedAt)
			SDL_Delay(static_cast<unsigned int>(milisecondsTonextFrame - (SDL_GetTicks() - frameStartedAt)));
	}
}

void SimulationManager::processInstruction(Instruction instructionIn) {
	Instruction instructionOut;
	Client* client = NULL;
	std::string argument = "";

	switch (instructionIn.getOpCode()) {
		case OPCODE_SIMULATION_SYNCHRONIZE:
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().getClient(argument);
			instructionOut.setOpCode(OPCODE_SIMULATION_SYNCHRONIZE);
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CONNECTED_AT,stringUtilities::unsignedToString(SDL_GetTicks()));
			client->addInstruction(instructionOut);
			client->setActive(true);
			break;
		case OPCODE_DISCONNECT_FROM_SIMULATION:
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().detachClient(argument);
			client->stopClient();
			delete client;
			std::cout << "THE USER " << argument << " DISCONNECTED FROM SIMULATION" << std::endl;
			break;
		case OPCODE_CLIENT_COMMAND: {
			std::cout << "PROCESSING COMMAND FROM CLIENT: " << instructionIn.serialize() << std::endl;
			std::string userID = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().getClient(userID);
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_COMMAND_DESTINATION);
			if (argument!="") {
				unsigned int deltaTime = SDL_GetTicks();
				//std::string movementArgument = GameView::instance().manageMovementUpdate(userID, argument, deltaTime);
				std::string movementArgument =  ""; //reemplazar x linea de arriba cuando compile GameView
				instructionOut.setOpCode(OPCODE_SIMULATION_UPDATE);
				std::string animation = "0";
				argument = userID+","+movementArgument+","+animation;
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SIMULATION_UPDATE, argument);
				this->getClients().addBroadcast(instructionOut);
			}
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_COMMAND_STATE);
			if (argument!="") {
				instructionOut.setOpCode(OPCODE_SIMULATION_UPDATE);
				std::string animation = argument;
				argument = userID+",0,"+animation;
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SIMULATION_UPDATE, argument);
				this->getClients().addBroadcast(instructionOut);
			}
			}
			break;
		case OPCODE_SIMULATION_UPDATE: {
			std::string userID = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_CURRENT_POSITION);
//			GameView::instance().managePositionUpdate(userID, argument);
			}
			break;
		case OPCODE_CONNECTION_ERROR: {
			std::cout << "THE USER " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID) << " DISCONECTED ABRUPTLY FROM SIMULATION" << std::endl;
			client = this->getClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
			if (client != NULL) {
				client->stopClient();
				delete client;
			}
			break;
		}
		default:
			std::cout << "INVALID OPCODE" << std::endl;
	}
}

void* SimulationManager::run() {
	this->simulate();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

ClientList& SimulationManager::getClients() {
	return this->clients;
}

InstructionQueue& SimulationManager::getInstructionQueue() {
	return this->instructionQueue;
}

bool SimulationManager::isStatusOk() {
	return this->statusOk;
}

std::string SimulationManager::getError() {
	return this->error;
}

void SimulationManager::startSimulationManager() {
	this->start();
}

void SimulationManager::stopSimulationManager() {
	//TODO: GET EVERY CLIENT AND STOP IT.
	this->setStopping(true);
	this->getInstructionQueue().stopWaiting();
	this->join();
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

SimulationManager::~SimulationManager() {
}
