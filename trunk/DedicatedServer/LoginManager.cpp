#include "LoginManager.h"

#include <iostream>

// ----------------------------------- CONSTRUCTOR ---------------------------------------

LoginManager::LoginManager(int portToListen, int maxPendingConnections, ChatManager& chatManager, SimulationManager& simulationManager) : listener(INADDR_ANY,portToListen,maxPendingConnections,this->getPreLoggedClients(),this->getInstructionQueue()), chatManager(chatManager), simulationManager(simulationManager) {
	this->statusOk = true;
	this->error = "";
	this->maxFileUpdaters = MAX_FILE_UPDATERS;
	this->getClientUpdaters().reserve(this->maxFileUpdaters);
	//SEE IF WORKS FINE.
	for (unsigned int i = 0; i < this->maxFileUpdaters; i++) {
		this->createClientUpdater();
	}
}

// ----------------------------------- PRIVATE METHODS -----------------------------------

ClientList& LoginManager::getPreLoggedClients() {
	return this->preLoggedClients;
}

ClientList& LoginManager::getLoggedClients() {
	return this->loggedClients;
}

InstructionQueue& LoginManager::getInstructionQueue() {
	return this->instructionQueue;
}

Listener& LoginManager::getListener() {
	return this->listener;
}

unsigned int LoginManager::getMaxFileUpdaters() {
	return this->maxFileUpdaters;
}

std::vector<ClientUpdater*>& LoginManager::getClientUpdaters() {
	return this->clientUpdaters;
}

ChatManager& LoginManager::getChatManager() {
	return this->chatManager;
}

SimulationManager& LoginManager::getSimulationManager() {
	return this->simulationManager;
}

void LoginManager::setStatusOk(bool statusOk) {
	this->statusOk = statusOk;
}

void LoginManager::setError(std::string error) {
	this->error = error;
}

void LoginManager::startListener() {
	this->getListener().startListening();
}

void LoginManager::stopListener() {
	this->getListener().stopListening();
}

//THIS METHOD SUCKS HARDER THAN "I HEART HUCKABEES". CLEAN UP
void LoginManager::processRequests() {
	Instruction instructionIn;
	Instruction instructionOut;
	Client* client = NULL;
	std::string argument = "";
	unsigned int index = 0;
	bool found = false;

	this->startListener();

	if (!this->getListener().isStatusOk()) {
		this->setStatusOk(false);
		this->setError(this->getListener().getError());
	}else{
		instructionIn = this->getInstructionQueue().getNextInstruction(true);
		while(!this->isStopping()) {
			instructionOut.clear();
			//TODO: USE LOGGER WHEN ITS FINISHED.
			std::cout << instructionIn.serialize() << std::endl;
			switch (instructionIn.getOpCode()) {
				case OPCODE_LOGIN_REQUEST:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID);
					if ( (argument != "") && (this->getLoggedClients().isUserIDAvailable(argument)) ) {
						string characterType = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_CHARACTER);
						if (GameView::instance().isCharacterTypeValid(characterType)) {
							instructionOut.setOpCode(OPCODE_LOGIN_OK);
							instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"Welcome " + argument);
							client = this->getPreLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
							client->setUserID(argument);
							this->getLoggedClients().addClient(client);
							GameView::instance().addPlayer(argument, characterType);
							std::cout << "THE USER " << argument << " LOGGED IN" << std::endl;
						}
						else {
							instructionOut.setOpCode(OPCODE_INVALID_CHARACTER);
							instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ERROR,"Invalid main character number");
							client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
							std::cout << "INVALID CHARACTER NUMBER FOR USER ID " << argument << std::endl;
						}
					} else if ((argument != "") && (!this->getLoggedClients().isUserIDAvailable(argument))) {
						instructionOut.setOpCode(OPCODE_LOGIN_OK);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"Welcome back " + argument);
						this->getPreLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						client = this->getLoggedClients().getClient(argument);
						client->addInstruction(instructionOut);
						instructionOut.clear();
						instructionOut.setOpCode(OPCODE_INIT_SYNCHRONIZE);
						string position = GameView::instance().managePlayerInitialSynchPosition(argument);
						string vision = GameView::instance().managePlayerInitialSynchVision(argument);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_CURRENT_POSITION, position);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_VISION, vision);
						std::cout << "THE USER " << argument << " LOGGED IN" << std::endl;
					}
					else {
						instructionOut.setOpCode(OPCODE_USERID_NOT_AVAILABLE);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ERROR,"Invalid user ID");
						client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						std::cout << "THE USERID " << argument << " IS INVALID" << std::endl;
						//instructionOut.setOpCode(OPCODE_USERID_NOT_AVAILABLE);
						//instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_ERROR,"User ID already in use");
						//client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						//std::cout << "THE USERID " << argument << " IS ALREADY IN USE" << std::endl;
					}
					client->addInstruction(instructionOut);
					break;
				case OPCODE_UPDATE_REQUEST:
					for (unsigned int i = 0; (i < this->getMaxFileUpdaters() && !found); i++) {
						if (this->getClientUpdaters()[i]->isAvailable()) {
							found = true;
							index = i;
						}
					}
					if (index < this->getMaxFileUpdaters()) {
						client = this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
						if (client != NULL) {// THIS CHECK SHOULD BE UNNECESARY.....
							std::cout << "THE USER " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID) << " IS UPDATING" << std::endl;
							this->getClientUpdaters()[index]->setClient(client);
							this->getClientUpdaters()[index]->startClientUpdater();
						}
					} else {
						instructionOut.setOpCode(OPCODE_SERVER_BUSY);
						//TODO: Add some lame ass message
						this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID))->addInstruction(instructionOut);
					}
					break;
				case OPCODE_CONNECT_TO_CHAT:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getPreLoggedClients().detachClient(argument);
					if ( (client != NULL) && (!this->getLoggedClients().isUserIDAvailable(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID))) ) {
						instructionOut.setOpCode(OPCODE_CHAT_CONNECTION_ESTABLISHED);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"SOME CHAT GREEETING MESSAGE");
						client->getConnector().setInstructionQueue(&this->getChatManager().getInstructionQueue());
						client->setUserID(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID));
						this->getChatManager().getClients().addClient(client);
						client->addInstruction(instructionOut);
						std::cout << "THE USER " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID) << " CONNECTED TO CHAT" << std::endl;
					} else {
						std::cout << "THIS SHOULD NEVER EVER EVER HAPPEN" << std::endl;
					}
					break;
				case OPCODE_CONNECT_TO_SIMULATION:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getPreLoggedClients().detachClient(argument);
					if ( (client != NULL) && (!this->getLoggedClients().isUserIDAvailable(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID))) ) {
						instructionOut.setOpCode(OPCODE_SIMULATION_CONNECTION_ESTABLISHED);
						instructionOut.insertArgument(INSTRUCTION_ARGUMENT_KEY_GREETING,"SOME SIMULATION GREEETING MESSAGE");
						client->getConnector().setInstructionQueue(&this->getSimulationManager().getInstructionQueue());
						client->setUserID(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID));
						this->getSimulationManager().getClients().addClient(client);
						client->addInstruction(instructionOut);
						std::cout << "THE USER " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_REQUESTED_USER_ID) << " CONNECTED TO SIMULATION" << std::endl;
					} else {
						std::cout << "THIS SHOULD NEVER EVER EVER HAPPEN EITHER" << std::endl;
					}
					break;
				case OPCODE_LOGOUT_REQUEST:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getLoggedClients().detachClient(argument);
					if (client != NULL) {
						std::cout << "THE USER " << argument << " LOGGED OUT" << std::endl;
						client->stopClient();
						delete client;
					} else {
						std::cout << "THIS IS ANOTHER THING THAT SHOULD NEVER EVER EVER HAPPEN" << std::endl;
					}
					break;
				case OPCODE_DISCONNECT:
					argument = instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID);
					client = this->getPreLoggedClients().detachClient(argument);
					if (client != NULL) {
						client->stopClient();
						delete client;
					} else {
						std::cout << "THIS IS ANOTHER THING THAT SHOULD NEVER EVER EVER HAPPEN" << std::endl;
					}
					break;
				case OPCODE_CONNECTION_ERROR:
					std::cout << "THE USER " << instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID) << " DISCONECTED ABRUPTLY" << std::endl;
					client = this->getPreLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
					if (client == NULL) {
						client = this->getLoggedClients().detachClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID));
					}
					client->stopClient();
					delete client;
					break;
				default:
					instructionOut.setOpCode(OPCODE_INVALID);
					this->getPreLoggedClients().getClient(instructionIn.getArgument(INSTRUCTION_ARGUMENT_KEY_USER_ID))->addInstruction(instructionOut);
				break;
			}
			//To avoid process an invalid instruction if server is shutting down.
			instructionIn = this->getInstructionQueue().getNextInstruction(true);
		}

		this->stopListener();

	}
}

void LoginManager::createClientUpdater() {
	ClientUpdater* clientUpdater = new ClientUpdater(this->getInstructionQueue());
	this->getClientUpdaters().push_back(clientUpdater);
}

void* LoginManager::run() {
	this->processRequests();
	return NULL;
}

// ----------------------------------- PUBLIC METHODS ------------------------------------

bool LoginManager::isStatusOk() {
	return this->statusOk;
}

std::string LoginManager::getError() {
	return this->error;
}

void LoginManager::startLoginManager() {
	this->start();
}

void LoginManager::stopLoginManager() {
	//TODO: GET EVERY CLIENT AND STOP IT.
	this->setStopping(true);
	this->getInstructionQueue().stopWaiting();
	this->join();
}

// ----------------------------------- DESTRUCTOR ----------------------------------------

LoginManager::~LoginManager() {
	for (unsigned int i = 0; i < this->getMaxFileUpdaters(); i++) {
		if (!this->getClientUpdaters()[i]->isAvailable())
			this->getClientUpdaters()[i]->stopClientUpdater();
		delete this->getClientUpdaters()[i];
	}
}
