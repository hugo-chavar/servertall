#include "SimulationManager.h"

#include "SDL.h"
#include "StringUtilities.h"
#include "GameView.h"
#include "CraPPyLog.h"

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
	Uint32 milisecondsTonextFrame = 1000/DESIREDFPS; 
	Uint32 frameStartedAt = 0;
	this->lastBroadcast = "";
	
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

		// AVANZO LA SIMULACIÓN UN DELTA DE TIEMPO.
		GameView::instance().update();

		// HACER UN BROADCAST DEL UPDATE A LOS CLIENTES
		instructionOut.clear();
		instructionOut.setOpCode(OPCODE_SIMULATION_UPDATE);
		std::string argument = GameView::instance().managePlayersUpdate();
		
		if (argument.size() > 0) {
			//Logger::instance().log("Argument "+argument);
			if (this->lastBroadcast != argument){
				this->lastBroadcast = argument;
				//argument.append(":");
				//argument.append(stringUtilities::unsignedToString(static_cast<unsigned>(SDL_GetTicks())));
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_SIMULATION_UPDATE, argument);
				argument = stringUtilities::unsignedToString(static_cast<unsigned>(SDL_GetTicks()));
				LOG_DEBUG("SIMULATION GENERATED AT: " + argument);
				instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CONNECTED_AT, argument);
				this->getClients().addBroadcast(instructionOut);

			}
		}
		i++;

		if (milisecondsTonextFrame >= SDL_GetTicks() - frameStartedAt)
			SDL_Delay(milisecondsTonextFrame - (SDL_GetTicks() - frameStartedAt));
	}
}

void SimulationManager::processInstruction(Instruction instructionIn) {
	Instruction instructionOut;
	Client* client = NULL;
	std::string argument = "";

	switch (instructionIn.getOpCode()) {
		case OPCODE_SYNCHRONIZE_CLOCK:
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().getClient(argument);
			instructionOut.setOpCode(OPCODE_SYNCHRONIZE_CLOCK);
			argument = stringUtilities::unsignedToString(static_cast<unsigned>(SDL_GetTicks()));
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CONNECTED_AT, argument);
			client->addInstruction(instructionOut);
			this->lastBroadcast = "";
			break;
		case OPCODE_SIMULATION_SYNCHRONIZE:
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().getClient(argument);
			instructionOut.setOpCode(OPCODE_SIMULATION_SYNCHRONIZE);
			argument = stringUtilities::unsignedToString(static_cast<unsigned>(SDL_GetTicks()));
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CONNECTED_AT, argument);
			client->addInstruction(instructionOut);
			this->lastBroadcast = "";
			break;
		case OPCODE_DISCONNECT_FROM_SIMULATION:
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().detachClient(argument);
			client->stopClient();
			delete client;
			LOG_DEBUG("THE USER " + argument + " DISCONNECTED FROM SIMULATION");
			GameView::instance().setDisconnectedPlayer(argument);
			break;
		case OPCODE_CLIENT_COMMAND: {
			LOG_DEBUG("PROCESSING COMMAND FROM CLIENT: " + instructionIn.serialize());
			std::string userID = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().getClient(userID);
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_COMMAND_DESTINATION);
			if (argument!="") {
				GameView::instance().manageMovementUpdate(userID, argument);

			}
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_COMMAND_STATE);
			if (argument!="") {
				GameView::instance().manageAnimationUpdate(userID, argument);
			}
			}
			break;
		case OPCODE_INIT_SYNCHRONIZE:{
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			client = this->getClients().getClient(argument);
			std::string characterType = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_CHARACTER);
			GameView::instance().addPlayer(argument, characterType);
			GameView::instance().startUpdatingPlayer(argument);
			client->setActive(true);

			instructionOut.clear();
			instructionOut.setOpCode(OPCODE_INIT_SYNCHRONIZE);
			std::string characterInit = GameView::instance().managePlayerInitialSynch(argument);
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CHARACTER_INIT, characterInit);
			std::string itemsInit = GameView::instance().getWorldView()->manageItemsInitialSynch();
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ITEMS_INIT,itemsInit);
			std::string missionInit = GameView::instance().getMission()->manageMissionInitialSynch();
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_Mission_INIT, missionInit);
			client->addInstruction(instructionOut);
			//broadcast for new players
			instructionOut.clear();
			instructionOut.setOpCode(OPCODE_CHARACTERS_SYNCHRONIZE);
			argument = GameView::instance().manageCharactersPlaying();
			instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CHARACTERS_UPDATE, argument);
			this->lastBroadcast = "";
			this->getClients().addBroadcast(instructionOut);
			}
			break;
		case OPCODE_SIMULATION_UPDATE:
			break;
		case OPCODE_CONNECTION_ERROR: {
			argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
			LOG_ERROR("THE USER " + argument + " DISCONECTED ABRUPTLY FROM SIMULATION");
			client = this->getClients().detachClient(argument);
			if (client != NULL) {
				client->stopClient();
				delete client;
			}
			GameView::instance().setDisconnectedPlayer(argument);
			break;
		}
		default:
			LOG_WARNING("INVALID OPCODE RECEIVED FROM CLIENT " + instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
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
	LOG_DEBUG("SIMULATOR MANAGER THREAD STARTED");
}

void SimulationManager::stopSimulationManager() {
	//TODO: GET EVERY CLIENT AND STOP IT.
	this->setStopping(true);
	this->getInstructionQueue().stopWaiting();
	this->join();
	LOG_DEBUG("SIMULATOR MANAGER THREAD STOPPED");
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

SimulationManager::~SimulationManager() {
}
